// commandinterpreter.cc
export module CommandInterpreter;

import <string>;
import <vector>;

using namespace std;

export class CommandInterpreter {
    // complete list of commands
    vector<string> validCommands;

    bool isPrefixOf(const string &input, const string &full) const;

    // for commands that ignore multipliers
    bool multiplierAllowed(const string &cmdWord) const;

public:
    CommandInterpreter();

    // returns stuff like "left", or "left left left", or "drop", or ""
    string parse(const string &cmd);

    // expands shortcuts:
    // - trims whitespace
    // - built-in aliases: cw → clockwise, ccw → counterclockwise, seq → sequence
    string expandShortcuts(const string &cmd);

    // applies a leading multiplier (3left → "left left left")
    // also resolves abbreviations internally
    string applyMultiplier(const string &cmd);

    // REMOVED, MAKES MORE SENSE IN GAMEENGINE
    // // reads a sequence file line-by-line (validation only)
    // void executeSequenceFile(const string &filename);

    // resolves "lef" → "left", "ri" → "right", etc.
    string resolveAbbreviations(const string &cmd);
};
