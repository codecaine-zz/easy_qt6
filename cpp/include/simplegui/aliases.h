#pragma once
// Familiar names for people coming from Delphi, Lazarus/Free Pascal, Visual Basic,
// WinForms, or V's simplegui/ui libraries.  Every alias is the *same* class under
// another name, so you can mix them freely:
//
//   auto memo = std::make_shared<simplegui::Memo>();      // same as Textarea
//   auto edit = std::make_shared<simplegui::Edit>("Hi");  // same as TextInput
//
// This file is included automatically by "simplegui/simplegui.h".

#include "simplegui/button.h"
#include "simplegui/checkbox.h"
#include "simplegui/color_well.h"
#include "simplegui/canvas.h"
#include "simplegui/date_picker.h"
#include "simplegui/grid.h"
#include "simplegui/group_box.h"
#include "simplegui/hbox.h"
#include "simplegui/image.h"
#include "simplegui/image_button.h"
#include "simplegui/knob.h"
#include "simplegui/label.h"
#include "simplegui/led_indicator.h"
#include "simplegui/link.h"
#include "simplegui/masked_input.h"
#include "simplegui/number_input.h"
#include "simplegui/password_input.h"
#include "simplegui/progress_indicator.h"
#include "simplegui/radial_gauge.h"
#include "simplegui/radio.h"
#include "simplegui/scroll_view.h"
#include "simplegui/slider.h"
#include "simplegui/split_view.h"
#include "simplegui/tab_view.h"
#include "simplegui/terminal_view.h"
#include "simplegui/text_input.h"
#include "simplegui/textarea.h"
#include "simplegui/toggle_switch.h"
#include "simplegui/vbox.h"
#include "simplegui/web_view.h"

namespace simplegui {

// Buttons
using CommandButton = Button;      // VB
using PushButton = Button;         // Qt / Win32
using BitBtn = ImageButton;        // Delphi / Lazarus TBitBtn
using SpeedButton = ImageButton;   // Delphi / Lazarus TSpeedButton

// Text
using StaticText = Label;          // Lazarus TStaticText
using Edit = TextInput;            // Delphi / Lazarus TEdit
using TextBox = TextInput;         // VB / WinForms
using LineEdit = TextInput;        // Qt
using PasswordEdit = PasswordInput;
using Memo = Textarea;             // Delphi / Lazarus TMemo
using TextArea = Textarea;         // HTML / V
using MaskEdit = MaskedInput;      // Delphi / Lazarus TMaskEdit
using HyperLink = Link;
using LinkLabel = Link;            // WinForms

// Choices
using CheckBox = Checkbox;         // Delphi / Lazarus / VB spelling
using RadioButton = Radio;
using OptionButton = Radio;        // VB
using Switch = ToggleSwitch;

// Numbers & ranges
using TrackBar = Slider;           // Delphi / Lazarus / WinForms
using Scale = Slider;
using SpinEdit = NumberInput;      // Lazarus TSpinEdit
using SpinBox = NumberInput;       // Qt
using NumericUpDown = NumberInput; // WinForms
using ProgressBar = ProgressIndicator;
using Dial = Knob;
using Gauge = RadialGauge;
using Led = LedIndicator;
using Lamp = LedIndicator;

// Dates & colors
using DateTimePicker = DatePicker; // Delphi / WinForms
using DateEdit = DatePicker;       // Lazarus TDateEdit
using ColorButton = ColorWell;     // Lazarus TColorButton
using ColorBox = ColorWell;

// Pictures & drawing
using Picture = Image;
using PictureBox = Image;          // VB / WinForms
using PaintBox = Canvas;           // Delphi / Lazarus TPaintBox

// Tables
using StringGrid = Grid;           // Delphi / Lazarus TStringGrid
using Table = Grid;
using DataGrid = Grid;

// Containers & layout
using Column = VBox;               // V ui.column
using Row = HBox;                  // V ui.row
using Panel = VBox;                // Delphi / Lazarus TPanel (children stacked top-to-bottom)
using Frame = GroupBox;            // VB Frame
using PageControl = TabView;       // Delphi / Lazarus TPageControl
using TabControl = TabView;        // WinForms
using Notebook = TabView;          // Lazarus TNotebook
using ScrollBox = ScrollView;      // Delphi / Lazarus TScrollBox
using ScrollArea = ScrollView;     // Qt
using Splitter = SplitView;        // Delphi / Lazarus TSplitter

// Other
using WebBrowser = WebView;        // Delphi / VB
using Console = TerminalView;

}  // namespace simplegui
