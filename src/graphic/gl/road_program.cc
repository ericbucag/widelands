/*
 * Copyright (C) 2006-2015 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "graphic/gl/road_program.h"

#include <cassert>
#include <cmath>

#include "base/log.h"
#include "graphic/gl/coordinate_conversion.h"
#include "graphic/gl/fields_to_draw.h"
#include "graphic/gl/utils.h"
#include "graphic/graphic.h"
#include "graphic/image_io.h"
#include "graphic/texture.h"
#include "logic/roadtype.h"

// We target OpenGL 2.1 for the desktop here.
RoadProgram::RoadProgram() {
	gl_program_.build("road");

	attr_position_ = glGetAttribLocation(gl_program_.object(), "attr_position");
	attr_texture_position_ = glGetAttribLocation(gl_program_.object(), "attr_texture_position");
	attr_brightness_ = glGetAttribLocation(gl_program_.object(), "attr_brightness");

	u_z_value_ = glGetUniformLocation(gl_program_.object(), "u_z_value");
	u_texture_ = glGetUniformLocation(gl_program_.object(), "u_texture");
}

RoadProgram::~RoadProgram() {
}

void RoadProgram::add_road(const int renderbuffer_width,
                           const int renderbuffer_height,
                           Widelands::Coords geometric_coords,
                           const FieldsToDraw::Field& start,
                           const FieldsToDraw::Field& end,
                           const float scale,
                           const Widelands::RoadType road_type,
                           const Direction direction,
                           uint32_t* gl_texture) {
	// The thickness of the road in pixels on screen.
	static constexpr float kRoadThicknessInPixels = 5.;

	// The overshot of the road in either direction in percent.
	static constexpr float kRoadElongationInPercent = .1;

	const float delta_x = end.surface_pixel.x - start.surface_pixel.x;
	const float delta_y = end.surface_pixel.y - start.surface_pixel.y;
	const float vector_length = std::hypot(delta_x, delta_y);

	const float road_overshoot_x = delta_x * kRoadElongationInPercent;
	const float road_overshoot_y = delta_y * kRoadElongationInPercent;

	// Find the reciprocal unit vector, so that we can calculate start and end
	// points for the quad that will make the road.
	const float road_thickness_x = (-delta_y / vector_length) * kRoadThicknessInPixels * scale;
	const float road_thickness_y = (delta_x / vector_length) * kRoadThicknessInPixels * scale;

	assert(start.owner != nullptr || end.owner != nullptr);

	Widelands::Player* visible_owner = start.owner;
	if (start.owner == nullptr) {
		visible_owner = end.owner;
	}

	const Image& texture =
	   road_type == Widelands::RoadType::kNormal ?
	      visible_owner->tribe().road_textures().get_normal_texture(
	         geometric_coords, direction) :
	      visible_owner->tribe().road_textures().get_busy_texture(geometric_coords, direction);
	if (*gl_texture == 0) {
		*gl_texture = texture.blit_data().texture_id;
	}
	// We assume that all road textures are in the same OpenGL texture, i.e. in
	// one texture atlas.
	assert(*gl_texture == texture.blit_data().texture_id);

	const Rectf texture_rect = to_gl_texture(texture.blit_data());

	vertices_.emplace_back(PerVertexData{
	   start.surface_pixel.x - road_overshoot_x + road_thickness_x,
	   start.surface_pixel.y - road_overshoot_y + road_thickness_y, texture_rect.x, texture_rect.y,
	   start.brightness,
	});
	pixel_to_gl_renderbuffer(
	   renderbuffer_width, renderbuffer_height, &vertices_.back().gl_x, &vertices_.back().gl_y);

	vertices_.emplace_back(PerVertexData{
	   start.surface_pixel.x - road_overshoot_x - road_thickness_x,
	   start.surface_pixel.y - road_overshoot_y - road_thickness_y, texture_rect.x,
	   texture_rect.y + texture_rect.h, start.brightness,
	});
	pixel_to_gl_renderbuffer(
	   renderbuffer_width, renderbuffer_height, &vertices_.back().gl_x, &vertices_.back().gl_y);

	vertices_.emplace_back(PerVertexData{
	   end.surface_pixel.x + road_overshoot_x + road_thickness_x,
	   end.surface_pixel.y + road_overshoot_y + road_thickness_y, texture_rect.x + texture_rect.w,
	   texture_rect.y, end.brightness,
	});
	pixel_to_gl_renderbuffer(
	   renderbuffer_width, renderbuffer_height, &vertices_.back().gl_x, &vertices_.back().gl_y);

	// As OpenGl does not support drawing quads in modern days and we have a
	// bunch of roads that might not be neighbored, we need to add two triangles
	// for each road. :(. Another alternative would be to use primitive restart,
	// but that is a fairly recent OpenGL feature.
	vertices_.emplace_back(vertices_.at(vertices_.size() - 2));
	vertices_.emplace_back(vertices_.at(vertices_.size() - 2));

	vertices_.emplace_back(PerVertexData{
	   end.surface_pixel.x + road_overshoot_x - road_thickness_x,
	   end.surface_pixel.y + road_overshoot_y - road_thickness_y, texture_rect.x + texture_rect.w,
	   texture_rect.y + texture_rect.h, end.brightness,
	});
	pixel_to_gl_renderbuffer(
	   renderbuffer_width, renderbuffer_height, &vertices_.back().gl_x, &vertices_.back().gl_y);
}

void RoadProgram::draw(const int renderbuffer_width,
                       const int renderbuffer_height,
                       const FieldsToDraw& fields_to_draw,
                       const float scale,
                       const float z_value) {
	vertices_.clear();

	uint32_t gl_texture = 0;
	for (auto cursor = fields_to_draw.cursor(); cursor.valid(); cursor.next()) {
		const FieldsToDraw::Field& field = cursor.field();

		// Road to right neighbor.
		if (cursor.rn_valid()) {
			const Widelands::RoadType road =
			   static_cast<Widelands::RoadType>(field.roads & Widelands::RoadType::kMask);
			if (road != Widelands::RoadType::kNone) {
				add_road(renderbuffer_width, renderbuffer_height, cursor.geometric_coords(), field,
				         cursor.rn(), scale, road, kEast, &gl_texture);
			}
		}

		// Road to bottom right neighbor.
		if (cursor.brn_valid()) {
			const Widelands::RoadType road =
			   static_cast<Widelands::RoadType>((field.roads >> 2) & Widelands::RoadType::kMask);
			if (road != Widelands::RoadType::kNone) {
				add_road(renderbuffer_width, renderbuffer_height, cursor.geometric_coords(), field,
				         cursor.brn(), scale, road, kSouthEast, &gl_texture);
			}
		}

		// Road to bottom right neighbor.
		if (cursor.bln_valid()) {
			const Widelands::RoadType road =
			   static_cast<Widelands::RoadType>((field.roads >> 4) & Widelands::RoadType::kMask);
			if (road != Widelands::RoadType::kNone) {
				add_road(renderbuffer_width, renderbuffer_height, cursor.geometric_coords(), field,
				         cursor.bln(), scale, road, kSouthWest, &gl_texture);
			}
		}
	}

	glUseProgram(gl_program_.object());

	auto& gl_state = Gl::State::instance();
	gl_state.enable_vertex_attrib_array({attr_position_, attr_texture_position_, attr_brightness_});

	gl_array_buffer_.bind();
	gl_array_buffer_.update(vertices_);

	Gl::vertex_attrib_pointer(
	   attr_position_, 2, sizeof(PerVertexData), offsetof(PerVertexData, gl_x));
	Gl::vertex_attrib_pointer(
	   attr_texture_position_, 2, sizeof(PerVertexData), offsetof(PerVertexData, texture_x));
	Gl::vertex_attrib_pointer(
	   attr_brightness_, 1, sizeof(PerVertexData), offsetof(PerVertexData, brightness));

	gl_state.bind(GL_TEXTURE0, gl_texture);
	glUniform1i(u_texture_, 0);
	glUniform1f(u_z_value_, z_value);

	glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
}
