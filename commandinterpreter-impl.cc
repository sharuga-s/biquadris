module CommandInterpreter;

import <fstream>;
import <sstream>;
import <string>;
import <vector>;
import <cctype>;

using namespace std;

CommandInterpreter::CommandInterpreter() {
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

bool CommandInterpreter::isPrefixOf(const string &input, const string &full) const {
    if (input.size() > full.size()) {
        return false;
    }
    
    for (int i = 0; i < input.size(); i++) {
        if (input[i] != full[i]) {
            return false;
        }
    }

    return true;
}

bool CommandInterpreter::multiplierAllowed(const string &cmdWord) const {
    if (cmdWord == "restart" || cmdWord == "random" || 
        cmdWord == "norandom" || cmdWord == "sequence" ||
        cmdWord == "blind"   || cmdWord == "heavy") {
        return false;
    }
    return true;
}

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

    // if entire string was whitespace → return empty
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

string CommandInterpreter::resolveAbbreviations(const string &cmd) {
    for (const string &full : validCommands) {
        if (cmd == full) {
            return cmd;
        }
    }

    string match = "";
    int count = 0;

    for (const string &full : validCommands) {
        if (isPrefixOf(cmd, full)) {
            match = full;
            count = count + 1;
        }
    }

    if (count == 1) { //we need it to be unique
        return match;
    }

    return cmd;
}

string CommandInterpreter::applyMultiplier(const string &cmd) {

    int i = 0;
    while (i < cmd.length() && isdigit(cmd[i])) {
        i = i + 1;
    }

    if (i == 0) {
        string resolved = resolveAbbreviations(cmd);
        return resolved;
    }

    int mult = 0;
    for (int j = 0; j < i; j++) {
        mult = mult * 10 + (cmd[j] - '0');
    }

    // idk if this is the best behaviour, revisit ****************
    if (mult == 0) {
        return "";
    }

    string rest = "";
    for (int j = i; j < cmd.length(); j++) {
        rest = rest + cmd[j];
    }

    string word = "";
    string args = "";

    int spacePos = -1;
    for (int j = 0; j < rest.length(); j++) {
        if (rest[j] == ' ') {
            spacePos = j;
            break;
        }
    }

    if (spacePos == -1) {
        // no args
        for (int j = 0; j < (int)rest.length(); j++) {
            word = word + rest[j];
        }
    } else {
        // build word
        for (int j = 0; j < spacePos; j++) {
            word = word + rest[j];
        }
        // build args (starts at the space)
        for (int j = spacePos; j < rest.length(); j++) {
            args = args + rest[j];
        }
    }

    string resolved = resolveAbbreviations(word);

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

string CommandInterpreter::parse(const string &cmd) {
    string expanded = expandShortcuts(cmd);
    string finalCmd = applyMultiplier(expanded);
    return finalCmd;
}