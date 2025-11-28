module CommandInterpreter;

import <fstream>;
import <sstream>;
import <string>;
import <vector>;
import <cctype>;

using namespace std;

CommandInterpreter::CommandInterpreter() {
    // dict of the valid commands -> abbreviations must map to one of these
    validCommands = {
        "left",
        "right",
        "down",
        "clockwise",
        "counterclockwise",
        "drop",
        "levelup",
        "leveldown",
        "restart",
        "random",
        "norandom",
        "sequence",
        "hold",
        "blind",
        "heavy",
        "I", "J", "L", "O", "S", "Z", "T"
    };
}

// purpose: true if `input` is a prefix of `full`
bool CommandInterpreter::isPrefixOf(const string &input, const string &full) const {
    if (input.size() > full.size()) {
        return false;
    }
    
    // size_t used in loop to surpress warnings (applies to all other loops too)
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] != full[i]) {
            return false;
        }
    }

    return true;
}

// purpose: some commands SHOULD NOT support multipliers
bool CommandInterpreter::multiplierAllowed(const string &cmdWord) const {
    if (cmdWord == "restart" || cmdWord == "random" || 
        cmdWord == "norandom" || cmdWord == "sequence" ||
        cmdWord == "blind"   || cmdWord == "heavy") {
        return false;
    }
    return true;
}

// special shorthand commands
string CommandInterpreter::expandShortcuts(const string &cmd) {
    //trim whitespace
    string cleaned = "";
    int start = 0;
    int end = cmd.length() - 1;

    while (start <= end && (cmd[start] == ' ' || cmd[start] == '\n')) {
        start = start + 1;
    }

    while (end >= start && (cmd[end] == ' ' || cmd[end] == '\n')) {
        end = end - 1;
    }

    // if entire string was whitespace, we return empty
    if (start > end) {
        return "";
    }

    //rebuild clean command
    for (int i = start; i <= end; i++) {
        cleaned = cleaned + cmd[i];
    }

    if (cleaned == "cw") {
        return "clockwise";
    }
    if (cleaned == "ccw") {
        return "counterclockwise";
    }
    if (cleaned == "seq") {
        return "sequence";
    }

    return cleaned;
}

// purpose: exact match, return it otherwise return original
string CommandInterpreter::resolveAbbreviations(const string &cmd) {
    // 1. exact command match
    for (const string &full : validCommands) {
        if (cmd == full) {
            return cmd;
        }
    }

    // 2. // prefix match search
    string match = "";
    int count = 0;

    for (const string &full : validCommands) {
        if (isPrefixOf(cmd, full)) {
            match = full;
            count = count + 1;
        }
    }

    if (count == 1) { // we need it to be unique
        return match;
    }

    // else, return ambig command
    return cmd;
}

// purpose: numeric multiplier for commands that can take it
string CommandInterpreter::applyMultiplier(const string &cmd) {

    size_t i = 0;
    while (i < cmd.length() && isdigit(cmd[i])) {
        i = i + 1;
    }

    // case 1: no digits -> resolve normall
    if (i == 0) {
        string resolved = resolveAbbreviations(cmd);
        return resolved;
    }

    // make leading digits into a multiplier
    int mult = 0;
    for (size_t j = 0; j < i; j++) {
        mult = mult * 10 + (cmd[j] - '0');
    }

    // ignore mults of 0
    if (mult == 0) {
        return "";
    }

    // separate command word and args
    string rest = "";
    for (size_t j = i; j < cmd.length(); j++) {
        rest = rest + cmd[j];
    }

    string word = "";
    string args = "";

    int spacePos = -1;
    for (size_t j = 0; j < rest.length(); j++) {
        if (rest[j] == ' ') {
            spacePos = static_cast<int>(j);
            break;
        }
    }

    if (spacePos == -1) {
        // no args
        for (size_t j = 0; j < rest.length(); j++) {
            word = word + rest[j];
        }
    } else {
        // build word
        for (size_t j = 0; j < static_cast<size_t>(spacePos); j++) {
            word = word + rest[j];
        }
        // build args (starts at the space)
        for (size_t j = spacePos; j < rest.length(); j++) {
            args = args + rest[j];
        }
    }

    // resolve abbreviations BEFORE checking multiplier validity
    string resolved = resolveAbbreviations(word);

    // check mult validity
    if (!multiplierAllowed(resolved)) {
        return resolved + args;
    }

    //apply mult
    string result = "";
    for (int k = 0; k < mult; k++) {
        result = result + resolved;
        if (k < mult - 1) {
            result = result + " ";
        }
    }

    return result;
}

// purpose: 
    // 1. expand special shortcuts
    // 2. apply numeric multipliers
    // 3. resolve abbreviations
string CommandInterpreter::parse(const string &cmd) {
    string expanded = expandShortcuts(cmd);
    string finalCmd = applyMultiplier(expanded);
    return finalCmd;
}
