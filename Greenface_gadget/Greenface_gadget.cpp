#include "Arduino.h"
#include <TerminalVT100.h>
#include <Greenface_EEPROM.h>
#include <Greenface_ui.h>

#include <Greenface_gadget.h>

extern uint16_t get_chars_per_line();
extern void noop();

// vector<string> split (const string &s, char delim) {
//     vector<string> result;
//     stringstream ss (s);
//     string item;

//     while (getline (ss, item, delim)) {
//         result.push_back (item);
//     }

//     return result;
// }

extern String csv_elem(String s, char delim, int instance);

Greenface_gadget::Greenface_gadget(String _name, String *_labels, uint16_t **_params, uint16_t _num_params) : params(*_params, _num_params)
{
    name = _name;
    labels = _labels;
    num_params = _num_params;
    param_num = 0;
    digit_num = 0;
    params = EEPROM_Arr16(_params[0], _num_params);
    mins = _params[1];
    maxs = _params[2];
    init_vals = _params[3];
    display_fxn = &noop;
}

void Greenface_gadget::begin()
{
    params.begin(false);
    params.xfer();
    update_fxns = nullptr;
    triggered = false;

    // EEPROM_String *string_var = &string_vars[get_param(p_num)];
    // Serial.println("Spank Begin: " + name);
    // Serial.println("Size: " + String(sizeof(string_vars)));

    // Serial.println("");
    // Serial.print(name + " Beginning "+labels[0]);
    // Serial.println(params.get(0));
}

int Greenface_gadget::offset()
{
    return params.offset;
}

uint8_t Greenface_gadget::get_param_type(int p_num)
{
    if (string_params)
    {
        if (string_params[p_num].length())
        {
            if (string_params[p_num] == "$~")
            {
                return SPANK_STRING_VAR_TYPE;
            }
            else if (string_params[p_num] == "#~")
            {
                return SPANK_FLOAT_VAR_TYPE;
            }
            else
            {
                return SPANK_STRING_PARAM_TYPE;
            }
        }
    }
    return SPANK_INT_PARAM_TYPE;
}

uint16_t Greenface_gadget::get_param(int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    uint16_t param = params.get(indx);
    param = min(get_max(indx), param);
    param = max(get_min(indx), param);
    return param;
}

bool Greenface_gadget::get_param_active(int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    if (active_params)
    {
        return active_params[indx];
    }
    else
    {
        return false;
    }
}

int Greenface_gadget::get_param_w_offset(int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    int param = (int)params.get(indx);
    if (offsets)
    {
        param += offsets[indx];
    }
    // hmmm... no constraints?
    // param = min(get_max(indx), param);
    // param = max(get_min(indx), param);
    return param;
}

String Greenface_gadget::get_param_as_string(int16_t indx)
{
    // uint16_t param = get_param(indx);
    return csv_elem(string_params[indx], ',', get_param(indx));
}

String Greenface_gadget::get_param_as_string_var(int16_t indx)
{
    EEPROM_String *string_var = &string_vars[get_param(indx)];
    return (*string_var).get();
}

void Greenface_gadget::put_string_var(String val, int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    uint8_t param_type = get_param_type(indx);
    if (param_type == SPANK_STRING_VAR_TYPE)
    {
        string_vars[get_param(indx)].put(val);
        printParam();
        exe_update_fxn();
        // Serial.println("indx: " + String(indx) + " param: " + String(get_param(indx)) + " val: " + val);
    }
}

uint16_t Greenface_gadget::get_min(int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    return mins[indx];
}

uint16_t Greenface_gadget::get_max(int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    return maxs[indx];
}

int Greenface_gadget::get_min_w_offset(int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    int16_t min = mins[indx];
    if (offsets)
    {
        min += offsets[indx];
    }
    return min;
}

int Greenface_gadget::get_max_w_offset(int16_t indx)
{
    if (indx == -1)
        indx = param_num;
    int16_t max = maxs[indx];
    if (offsets)
    {
        max += offsets[indx];
    }
    return max;
}

void Greenface_gadget::adjust_param(int e, unsigned long delta)
{
    char string_var_char;
    EEPROM_String *string_var = &string_vars[get_param(param_num)];
    switch (get_param_type(param_num))
    {
    case SPANK_STRING_VAR_TYPE:
        string_var_char = (*string_var).charAt(digit_num);
        // Serial.println("string: " + string_var.get());
        string_var_char += e;
        if (string_var_char > 0x7b)
        {
            string_var_char = 0x20;
        }
        if (string_var_char < 0x20)
        {
            string_var_char = 0x7b;
        }

        // ui->terminal_debug("varchar: " + String(string_var_char) + " strlen: " + String((*string_var).length()));
        if (digit_num >= (*string_var).length())
        {
            (*string_var).append(String(string_var_char));
        }
        else
        {
            (*string_var).putCharAt((string_var_char), digit_num);
        }
        printParam();
        break;
    default:
        int the_param = get_param();
        int multiplier = 1;
        uint8_t ndigs = get_num_digits(param_num);
        int dig = (ndigs - digit_num) - 1;
        // Serial.println("mult: "+String(dig));
        multiplier = pow(10, dig);
        the_param += e * multiplier;
        // Serial.println("param: " + String(the_param));
        the_param = constrain(the_param, 0, 32767);
        put_param(the_param);
        break;
    }
}

int Greenface_gadget::check_param(int param)
{
    // ui.terminal_debug("Param OK param#: " + String(param_num) + " Offset: " + String(display_offset));
    switch (param_num)
    {
    case LONGEST_PULSE:
        param = max(param, get_param(SHORTEST_PULSE) + 1);
        break;
    case SHORTEST_PULSE:
        param = min(param, get_param(LONGEST_PULSE) - 1);
        break;
    }
    return param;
}

void Greenface_gadget::print_string_param(uint16_t p_num, uint16_t line_num)
{
    String s = csv_elem(string_params[p_num], ',', get_param(p_num));
    String sval = get_label(p_num) + s;
    int num_blanks = 20 - sval.length();
    for (int i = 0; i < num_blanks; i++)
    {
        sval += " ";
    }
    ui.printLine(sval, ui.lines[line_num], 1); // is 10 trailing spaces enough?
}

void Greenface_gadget::print_string_var(uint16_t p_num, uint16_t line_num)
{
    EEPROM_String *string_var = &string_vars[get_param(p_num)];
    String label = get_label(p_num);
    int label_len = label.length();
    String pad;
    int num_blanks = get_chars_per_line() - (label_len + (*string_var).length());
    for (int i = 0; i < num_blanks; i++)
    {
        pad.concat(' ');
    }
    ui.printLine(label + (*string_var).get() + pad, ui.lines[line_num], 1);
}

void Greenface_gadget::print_param(uint16_t p_num, uint16_t line_num)
{
    uint8_t param_type = get_param_type(p_num);
    float param_float;
    String label = get_label(p_num);

    if (alt_values)
    {
        String alt_value = alt_values[p_num];
        if (alt_value.length() > 0)
        {
            ui.printLine(label + alt_value, ui.lines[line_num], 1);
            return;
        }
    }

    switch (param_type)
    {
    case SPANK_STRING_PARAM_TYPE:
        print_string_param(p_num, line_num);
        break;
    case SPANK_STRING_VAR_TYPE:
        print_string_var(p_num, line_num);
        break;
    case SPANK_FLOAT_VAR_TYPE:
        param_float = (float)get_param(p_num);
        param_float = -5.0;
        ui.printLine(label + ui.format_float(param_float, 5), ui.lines[line_num], 1);
        // ui.printParam(get_label(p_num), get_param(p_num), ndigs, formatf, line_num);
        break;
    default:
        uint8_t ndigs = get_num_digits(p_num);
        String fmt = calc_format(ndigs);
        const char *format = fmt.c_str();
        int param = get_param(p_num);
        if (offsets)
        {
            param += offsets[p_num];
        }
        ui.printParam(get_label(p_num), param, ndigs, format, line_num);
        break;
    }
}

void Greenface_gadget::exe_update_fxn()
{
    if (update_fxns)
    {
        if (update_fxns[param_num] != nullptr)
        {
            (*update_fxns[param_num])();
        }
    }
}

// TODO fix data type to allow negative numbers
void Greenface_gadget::put_param_w_offset(int val, int8_t _param_num)
{
    uint16_t param;

    if (_param_num != -1)
    {
        param_num = _param_num;
    }
    if (offsets)
    {
        val -= offsets[param_num];
    }
    param = (uint16_t)val;
    put_param(param, param_num);
}

void Greenface_gadget::param_put(uint16_t val, int8_t _param_num)
{
    params.put(val, _param_num);
}

void Greenface_gadget::put_param(uint16_t val, int8_t _param_num)
{
    String s;
    int save_param_num = param_num;
    if (_param_num != -1)
    {
        param_num = _param_num;
    }
    if (val > get_max())
        val = get_max();
    if (val < get_min() || val > 32767)
        val = get_min();
    // Serial.print("final val: ");
    // Serial.println(val);
    if (check_params)
        val = check_param(val);

    // ui->terminal_debug("Param num: " + String(_param_num) + " Val: " + String(val));
    param_put(val, param_num);
    printParam();
    exe_update_fxn();
    param_num = save_param_num;
}

void Greenface_gadget::hilight_param()
{
    if (enable_hilight)
    {
        uint8_t dig_offset = get_label(param_num).length();
        int char_num = digit_num + dig_offset;
        ui.underline_char(char_num, min(2, param_num + display_offset), 1, 8);
        // ui.hiliteChar(min(2, param_num + display_offset), char_num);
        //  Serial.print ("Dig offset: "+String(dig_offset));
        //  Serial.print (" Digit #: "+String(digit_num));
        //  Serial.println(" Char Num: "+String(char_num));
    }
}

void Greenface_gadget::put_param_num(int val)
{
    int _val = val;
    _val = max(0, _val);
    _val = min(num_params - 1, _val);
    param_num = _val;
    digit_num = 0; // might only have one digit
}

void Greenface_gadget::insert_char(char c)
{
    uint8_t param_type = get_param_type(param_num);
    if (param_type == SPANK_STRING_VAR_TYPE)
    {
        String s = string_vars[get_param(param_num)].get();
        string_vars[get_param(param_num)].put(s.substring(0, digit_num) + String(c) + s.substring(digit_num));
        printParam();
    }
}

void Greenface_gadget::remove_char()
{
    uint8_t param_type = get_param_type(param_num);
    if (param_type == SPANK_STRING_VAR_TYPE)
    {
        String s = string_vars[get_param(param_num)].get();
        s.remove(digit_num - 1, 1);
        string_vars[get_param(param_num)].put(s);
        inc_dig_num_by(-1);
        printParam();
    }
}

void Greenface_gadget::inc_param_num_by(int val)
{
    if (param_num == 0 && val == -1)
    {
        param_num = num_params - 1;
    }
    else
    {
        param_num += val;
        if (param_num > num_params - 1)
            param_num = 0;
    }
    digit_num = 0;
    display();
    // printParams();
    // ui.terminal_debug("Inc param num by: " + String(val));
}

void Greenface_gadget::put_dig_num(int val)
{
    // uint8_t num_digs = log10(get_max(param_num))+1;
    uint16_t num_digs = get_num_digits(param_num);
    if (val < 0)
        val = 0;
    if (val > num_digs - 1)
        val = num_digs - 1;
    // Serial.println("Dig# "+String(digit_num)+" Digs "+String(num_digs));
    digit_num = val;
    printParam();
    hilight_param();
}

void Greenface_gadget::inc_dig_num_by(int val)
{
    // uint8_t num_digs = log10(get_max(param_num))+1;
    if (get_max() >= 10 || get_param_type(param_num) == SPANK_STRING_VAR_TYPE)
    {
        uint16_t num_digs = get_num_digits(param_num);
        digit_num += val;
        if (digit_num < 0)
            digit_num = 0;
        if (digit_num > num_digs - 1)
            digit_num = 0;
        // Serial.println("Dig# " + String(digit_num) + " Digs " + String(num_digs));
        printParam();
        hilight_param();
    }
}

String Greenface_gadget::get_label(int label_num)
{
    return labels[label_num];
}

void Greenface_gadget::printParam(int8_t _param_num)
{
    int save_param_num = param_num;
    if (_param_num != -1)
    {
        param_num = _param_num;
    }
    print_param(param_num, min(2, param_num + display_offset));
    param_num = save_param_num;
}

void Greenface_gadget::printParams()
{
    ui.fill(BLACK, 16 + (16 * display_offset));
    ui.t.clrDown(display_offset == 1 ? "10" : "8");
    hilight_param();
    // Serial.println("Num params: "+String(num_params) + " Param# "+String(param_num));
    int start = max(0, param_num - (2 - display_offset));
    int end = min(3 - display_offset, num_params);
    for (int i = 0; i < end; i++)
    {
        print_param(i + start, i + display_offset);
    }
}

String Greenface_gadget::params_toJSON()
{
    String out = "";
    String s, label;
    uint8_t param_type;

    for (int i = 0; i < num_params; i++)
    {
        if (out > "")
            out += ",";
        out += "{ ";
        label = get_label(i);
        if (label.startsWith("Idle Value"))
        {
            label.replace("Idle Value", "Idle_Value");
        }
        out += toJSON("label", label);
        out += ", ";

        param_type = get_param_type(i);
        switch (param_type)
        {
        case SPANK_STRING_PARAM_TYPE:
            out += toJSON("type", "select");
            out += ", ";
            out += toJSON("values", string_params[i]);
            out += ", ";
            out += toJSON("value", csv_elem(string_params[i], ',', get_param(i)));
            break;
        case SPANK_STRING_VAR_TYPE:
            out += toJSON("type", "text");
            out += ", ";
            out += toJSON("value", get_param_as_string_var(i));
            break;
        default:
            out += toJSON("type", "number");
            out += ", ";
            out += toJSON("value", String(get_param_w_offset(i)));
            break;
        }

        out += ", ";
        out += toJSON("numeric_val", String(get_param(i)));
        out += ", ";
        out += toJSON("param_num", String(i));
        out += ", ";
        out += toJSON("min", String(get_min_w_offset(i)));
        out += ", ";
        out += toJSON("max", String(get_max_w_offset(i)));
        out += ", ";
        int num_digits = get_num_digits(i);
        if (get_min_w_offset(i) < 0)
        {
            num_digits += -1;
        }
        out += toJSON("num_digits", String(num_digits));
        out += ", ";
        out += toJSON("selected", enable_hilight && param_num == i ? "true" : "false");
        out += " }";
    }
    return "[" + out + "]";
}

void Greenface_gadget::default_display()
{
    ui.newFxn(name);
    printParams();
}

void Greenface_gadget::display()
{
    if (display_fxn == &noop)
    {
        default_display();
    }
    display_fxn();
}

void Greenface_gadget::copy_to(Greenface_gadget &target)
{
    for (int i = 0; i < 6; i++)
    {
        target.params.put(0, i);
        target.labels[i] = "";
        target.mins[i] = 0;
        target.maxs[i] = 0;
    }
    for (int i = 0; i < num_params; i++)
    {
        target.params.put(get_param(i), i);
        target.labels[i] = get_label(i);
        target.mins[i] = get_min(i);
        target.maxs[i] = get_max(i);
    }
    target.name = name;
    target.trigger_fxn = trigger_fxn;
    target.num_params = num_params;
    target.string_params = string_params;
    target.check_params = check_params;
    // Serial.println("Copying: " + target.name + " Num params: " + String(target.num_params));
}

void Greenface_gadget::init()
{
    // ui.terminal_debug("Initializing: " + name);
    Serial.println("\nInitializing: " + name);
    param_num = 0;
    digit_num = 0;
    triggered = false;
    for (int i = 0; i < num_params; i++)
    {
        Serial.println(get_label(i) + String(init_vals[i]));
        params.put(init_vals[i], i);
    }
}

void Greenface_gadget::debug()
{
    Serial.println("Howdy I'm " + name);
    Serial.println("Initial Delay: " + String(get_param(INITIAL_DELAY)));
}

void Greenface_gadget::trigger(uint8_t by)
{
    triggered_by = by;
    triggered = !triggered;
    // if (triggered)
    // {
    //     // triggered = true;
    //     // Serial.println(name + " was triggered!");
    // }
    // else
    // {
    //     // Serial.println(name + " was un-triggered!");
    // }
}

void Greenface_gadget::clear_trigger(uint8_t by)
{
    if (triggered_by == by)
        triggered = false;
}

// private fxns
uint8_t Greenface_gadget::get_num_digits(int this_param_num)
{
    // Serial.println("Getting ndigs: "+String(this_param_num));
    int param_type = get_param_type(this_param_num);
    switch (param_type)
    {
    case SPANK_STRING_PARAM_TYPE:
        return 1;
        break;
    case SPANK_STRING_VAR_TYPE:
        // string_var = string_vars[get_param(this_param_num)];
        return string_vars[get_param(this_param_num)].size;
        break;
    default:
        int pad = offsets[this_param_num] < 0 ? 2 : 1;
        return log10(get_max(this_param_num)) + pad;
        break;
    }
}

String Greenface_gadget::calc_format(uint8_t ndigs)
{
    // uint8_t ndigs = get_num_digits(indx);
    return "%0" + String(ndigs) + "d";
    // const char* format = fmt.c_str();
    // return format;
}