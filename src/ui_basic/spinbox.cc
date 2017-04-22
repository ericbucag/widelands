/*
 * Copyright (C) 2009-2017 by the Widelands Development Team
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "ui_basic/spinbox.h"

#include <map>
#include <vector>

#include <boost/format.hpp>

#include "base/i18n.h"
#include "base/log.h"
#include "base/macros.h"
#include "base/wexception.h"
#include "graphic/font_handler1.h"
#include "graphic/text/font_set.h"
#include "graphic/text_constants.h"
#include "ui_basic/button.h"
#include "ui_basic/multilinetextarea.h"
#include "ui_basic/textarea.h"

namespace UI {

struct SpinBoxImpl {
	/// Value hold by the spinbox
	int32_t value;

	/// Minimum and maximum that \ref value may reach
	int32_t min;
	int32_t max;

	/// List of possible values for type kValueList
	std::vector<int32_t> values;

	/// The unit of the value
	UI::SpinBox::Units unit;

	/// Background tile style of buttons.
	const Image* background;

	/// Special names for specific values
	std::map<int32_t, std::string> value_replacements;

	/// The UI parts
	Textarea* text;
	UI::MultilineTextarea* label;
	Button* button_plus;
	Button* button_minus;
	Button* button_ten_plus;
	Button* button_ten_minus;
};

/**
 * SpinBox constructor:
 *
 * initializes a new spinbox with either two (big = false) or four (big = true)
 * buttons. w must be >= the space taken up by the buttons, else the spinbox would become useless
 * and so
 * throws an exception.
 * The spinbox' height and button size is set automatically according to the height of its textarea.
 */
SpinBox::SpinBox(Panel* const parent,
                 const int32_t x,
                 const int32_t y,
                 const uint32_t w,
                 const uint32_t unit_w,
                 int32_t const startval,
                 int32_t const minval,
                 int32_t const maxval,
                 const std::string& label_text,
                 const SpinBox::Units& unit,
                 const Image* button_background,
                 SpinBox::Type type,
                 int32_t step_size,
                 int32_t big_step_size)
   : Panel(parent, x, y, std::max(w, unit_w), 0),
     type_(type),
     sbi_(new SpinBoxImpl),
     unit_width_(unit_w),
     button_height_(20),
     padding_(2),
     number_of_paddings_(type_ == SpinBox::Type::kBig ? 6 : 4) {
	if (type_ == SpinBox::Type::kValueList) {
		sbi_->min = 0;
		sbi_->max = 0;
	} else {
		sbi_->min = minval;
		sbi_->max = maxval;
	}
	sbi_->value = startval;
	sbi_->unit = unit;
	sbi_->background = button_background;

	box_ = new UI::Box(this, 0, 0, UI::Box::Horizontal, 0, 0, padding_);

	sbi_->label =
	   new UI::MultilineTextarea(box_, 0, 0, 0, 0, label_text, UI::Align::kLeft, button_background,
	                             UI::MultilineTextarea::ScrollMode::kNoScrolling);
	box_->add(sbi_->label);

	sbi_->text = new UI::Textarea(box_, "", UI::Align::kCenter);

	bool is_big = type_ == SpinBox::Type::kBig;

	uint32_t padding = 2;
	uint32_t actual_w = std::max(w, unit_w);
	uint32_t no_padding = (is_big ? 6 : 4);
	// Give some height margin = 2 to keep the label from generating a scrollbar.
	uint32_t texth =
	   UI::g_fh1->render(as_uifont(UI::g_fh1->fontset()->representative_character()))->height() + 2;

	// 40 is an ad hoc width estimate for the MultilineTextarea scrollbar + a bit of text.
	if (!label_text.empty() && (w + padding) <= unit_w - 40) {
		throw wexception(
		   "SpinBox: Overall width %d must be bigger than unit width %d + %d * %d + 40 for padding",
		   w, unit_w, no_padding, padding);
	}

	if (unit_w < (is_big ? 7 * button_height_ : 3 * button_height_)) {
		log("Not enough space to draw spinbox \"%s\".\n"
		    "Width %d is smaller than required width %d."
		    "Please report as a bug.\n",
		    label_text.c_str(), unit_w, (is_big ? 7 * button_height_ : 3 * button_height_));
	}

	box_ = new UI::Box(this, 0, 0, UI::Box::Horizontal, actual_w, texth, padding);

	UI::MultilineTextarea* label = new UI::MultilineTextarea(
	   box_, 0, 0, w - unit_w - no_padding * padding, texth, label_text, UI::Align::kLeft,
	   button_background, UI::MultilineTextarea::ScrollMode::kNoScrolling);
	box_->add(label, Box::Resizing::kAlign, UI::Align::kCenter);

	sbi_->text = new UI::Textarea(box_, "", UI::Align::kCenter);

	sbi_->button_minus = new Button(
	   box_, "-", 0, 0, button_height_, button_height_, sbi_->background,
	   g_gr->images().get(is_big ?
	                         UI::g_fh1->fontset()->is_rtl() ? "images/ui_basic/scrollbar_right.png" :
	                                                          "images/ui_basic/scrollbar_left.png" :
	                         "images/ui_basic/scrollbar_down.png"),
	   _("Decrease the value"));
	sbi_->button_plus = new Button(
	   box_, "+", 0, 0, button_height_, button_height_, sbi_->background,
	   g_gr->images().get(is_big ?
	                         UI::g_fh1->fontset()->is_rtl() ? "images/ui_basic/scrollbar_left.png" :
	                                                          "images/ui_basic/scrollbar_right.png" :
	                         "images/ui_basic/scrollbar_up.png"),
	   _("Increase the value"));

	if (is_big) {
		sbi_->button_ten_minus =
		   new Button(box_, "--", 0, 0, 2 * button_height_, button_height_, sbi_->background,
		              g_gr->images().get(UI::g_fh1->fontset()->is_rtl() ?
		                                    "images/ui_basic/scrollbar_right_fast.png" :
		                                    "images/ui_basic/scrollbar_left_fast.png"),
		              _("Decrease the value by 10"));
		sbi_->button_ten_plus =
		   new Button(box_, "++", 0, 0, 2 * button_height_, button_height_, sbi_->background,
		              g_gr->images().get(UI::g_fh1->fontset()->is_rtl() ?
		                                    "images/ui_basic/scrollbar_left_fast.png" :
		                                    "images/ui_basic/scrollbar_right_fast.png"),
		              _("Increase the value by 10"));

		sbi_->button_ten_plus->sigclicked.connect(
		   boost::bind(&SpinBox::change_value, boost::ref(*this), big_step_size));
		sbi_->button_ten_minus->sigclicked.connect(
		   boost::bind(&SpinBox::change_value, boost::ref(*this), -1 * big_step_size));
		sbi_->button_ten_plus->set_repeating(true);
		sbi_->button_ten_minus->set_repeating(true);
		buttons_.push_back(sbi_->button_ten_minus);
		buttons_.push_back(sbi_->button_ten_plus);

		box_->add(sbi_->button_ten_minus);
		box_->add(sbi_->button_minus);
		box_->add(sbi_->text);
		box_->add(sbi_->button_plus);
		box_->add(sbi_->button_ten_plus);
	} else {
		box_->add(sbi_->button_minus);
		box_->add(sbi_->text);
		box_->add(sbi_->button_plus);
	}

	sbi_->button_plus->sigclicked.connect(
	   boost::bind(&SpinBox::change_value, boost::ref(*this), step_size));
	sbi_->button_minus->sigclicked.connect(
	   boost::bind(&SpinBox::change_value, boost::ref(*this), -1 * step_size));
	sbi_->button_plus->set_repeating(true);
	sbi_->button_minus->set_repeating(true);
	buttons_.push_back(sbi_->button_minus);
	buttons_.push_back(sbi_->button_plus);

	layout();
	update();
}

SpinBox::~SpinBox() {
	delete sbi_;
	sbi_ = nullptr;
}

void SpinBox::layout() {
	// Do not layout if the size hasn't been set yet.
	if (get_w() == 0 && get_h() == 0) {
		return;
	}
// NOCOM options screen throws the error
	// 40 is an ad hoc width estimate for the MultilineTextarea scrollbar + a bit of text.
	if (!sbi_->label->get_text().empty() && (get_w() + padding_) <= unit_width_ - 40) {
		throw wexception("SpinBox: Overall width %d must be bigger than unit width %d + %d * %d + "
		                 "40 for padding",
		                 get_w(), unit_width_, number_of_paddings_, padding_);
	}

	if (unit_width_ < (type_ == SpinBox::Type::kBig ? 7 * button_height_ : 3 * button_height_)) {
		log("Not enough space to draw spinbox \"%s\".\n"
		    "Width %d is smaller than required width %d."
		    "Please report as a bug.\n",
		    sbi_->label->get_text().c_str(), unit_width_,
		    (type_ == SpinBox::Type::kBig ? 7 * button_height_ : 3 * button_height_));
	}

	// 10 is arbitrary, the actual height will be set by the Multilinetextarea itself
	sbi_->label->set_size(get_w() - unit_width_ - number_of_paddings_ * padding_, 10);
	if (type_ == SpinBox::Type::kBig) {
		sbi_->text->set_fixed_width(unit_width_ - 2 * sbi_->button_ten_plus->get_w() -
		                            2 * sbi_->button_minus->get_w() - 2 * padding_);
	} else {
		sbi_->text->set_fixed_width(unit_width_ - 2 * sbi_->button_minus->get_w());
	}

	uint32_t box_height = std::max(sbi_->label->get_h(), static_cast<int32_t>(button_height_));
	box_->set_size(get_w(), box_height);
	set_desired_size(get_w(), box_height);
	set_size(get_w(), box_height);
}

/**
 * private function - takes care about all updates in the UI elements
 */
void SpinBox::update() {
	if (sbi_->value_replacements.count(sbi_->value) == 1) {
		sbi_->text->set_text(sbi_->value_replacements.at(sbi_->value));
	} else {
		if (type_ == SpinBox::Type::kValueList) {
			if ((sbi_->value >= 0) && (sbi_->values.size() > static_cast<size_t>(sbi_->value))) {
				sbi_->text->set_text(unit_text(sbi_->values.at(sbi_->value)));
			} else {
				sbi_->text->set_text(
				   "undefined");  // The user should never see this, so we're not localizing
			}
		} else {
			sbi_->text->set_text(unit_text(sbi_->value));
		}
	}

	sbi_->button_minus->set_enabled(sbi_->min < sbi_->value);
	sbi_->button_plus->set_enabled(sbi_->value < sbi_->max);
	if (type_ == SpinBox::Type::kBig) {
		sbi_->button_ten_minus->set_enabled(sbi_->min < sbi_->value);
		sbi_->button_ten_plus->set_enabled(sbi_->value < sbi_->max);
	}
}

/**
 * Display width for the spinbox unit
 * \note This does not relayout the spinbox.
 */
void SpinBox::set_unit_width(uint32_t width) {
	unit_width_ = width;
}

/**
 * private function called by spinbox buttons to in-/decrease the value
 */
void SpinBox::change_value(int32_t const value) {
	set_value(value + sbi_->value);
	changed();
}

/**
 * manually sets the used value to a given value
 */
void SpinBox::set_value(int32_t const value) {
	sbi_->value = value;
	if (sbi_->value > sbi_->max)
		sbi_->value = sbi_->max;
	else if (sbi_->value < sbi_->min)
		sbi_->value = sbi_->min;
	update();
}

void SpinBox::set_value_list(const std::vector<int32_t>& values) {
	sbi_->values = values;
	sbi_->min = 0;
	sbi_->max = values.size() - 1;
	update();
}

/**
 * sets the interval the value may lay in and fixes the value, if outside.
 */
void SpinBox::set_interval(int32_t const min, int32_t const max) {
	sbi_->max = max;
	sbi_->min = min;
	if (sbi_->value > max)
		sbi_->value = max;
	else if (sbi_->value < min)
		sbi_->value = min;
	update();
}

/**
 * \returns the value
 */
int32_t SpinBox::get_value() const {
	if (type_ == SpinBox::Type::kValueList) {
		if ((sbi_->value >= 0) && (sbi_->values.size() > static_cast<size_t>(sbi_->value))) {
			return sbi_->values.at(sbi_->value);
		} else {
			return -1;
		}
	} else {
		return sbi_->value;
	}
}

/**
 * Adds a replacement text for a specific value
 * overwrites an old replacement if one exists.
 */
void SpinBox::add_replacement(int32_t value, const std::string& text) {
	sbi_->value_replacements[value] = text;
	update();
}

const std::string SpinBox::unit_text(int32_t value) const {
	switch (sbi_->unit) {
	case (Units::kMinutes):
		/** TRANSLATORS: A spinbox unit */
		return (boost::format(ngettext("%d minute", "%d minutes", value)) % value).str();
	case (Units::kPixels):
		/** TRANSLATORS: A spinbox unit */
		return (boost::format(ngettext("%d pixel", "%d pixels", value)) % value).str();
	case (Units::kPercent):
		/** TRANSLATORS: A spinbox unit */
		return (boost::format(_("%i %%")) % value).str();
	case (Units::kNone):
		return (boost::format("%d") % value).str();
	}
	NEVER_HERE();
}

}  // namespace UI
