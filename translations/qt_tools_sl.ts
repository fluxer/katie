<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="sl">
<context>
    <name>AppFontDialog</name>
    <message>
        <location filename="../src/tools/designer/appfontdialog.cpp" line="+418"/>
        <source>Additional Fonts</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AppFontManager</name>
    <message>
        <location line="-267"/>
        <source>&apos;%1&apos; is not a file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>The font file &apos;%1&apos; does not have read permissions.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The font file &apos;%1&apos; is already loaded.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The font file &apos;%1&apos; could not be loaded.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+17"/>
        <source>&apos;%1&apos; is not a valid font id.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>There is no loaded font matching the id &apos;%1&apos;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>The font &apos;%1&apos; (%2) could not be unloaded.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AppFontWidget</name>
    <message>
        <location line="+26"/>
        <source>Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+58"/>
        <source>Add font files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Remove current font file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Remove all font files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Add Font Files</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Font files (*.ttf)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Error Adding Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Error Removing Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Remove Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Would you like to remove all fonts?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>AppearanceOptionsWidget</name>
    <message>
        <location filename="../src/tools/designer/qdesigner_appearanceoptions.ui"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>User Interface Mode</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FindDialog</name>
    <message>
        <source></source>
        <comment>Choose Edit|Find from the menu bar or press Ctrl+F to pop up the Find dialog</comment>
        <translation></translation>
    </message>
</context>
<context>
    <name>FontPanel</name>
    <message>
        <location filename="../src/tools/designer/fontpanel/fontpanel.cpp" line="+63"/>
        <source>Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>&amp;Writing system</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&amp;Family</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&amp;Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&amp;Point size</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LConvert</name>
    <message>
        <location filename="../src/tools/lconvert/lconvert.cpp" line="+68"/>
        <source>
Usage:
    lconvert [options] &lt;infile&gt; [&lt;infile&gt;] ...

lconvert is part of Katie&apos;s Linguist tool chain. It can be used as a
stand-alone tool to convert and filter translation data files.
The following file formats are supported:

%1
If multiple input files are specified, they are merged with
translations from later files taking precedence.

Options:
    -h
    --help
           Display this information and exit.

    -i &lt;infile&gt;
    --input-file &lt;infile&gt;
           Specify input file. Use if &lt;infile&gt; might start with a dash.
           This option can be used several times to merge inputs.
           May be &apos;-&apos; (standard input) for use in a pipe.

    -o &lt;outfile&gt;
    --output-file &lt;outfile&gt;
           Specify output file. Default is &apos;-&apos; (standard output).

    -if &lt;informat&gt;
    --input-format &lt;format&gt;
           Specify input format for subsequent &lt;infile&gt;s.
           The format is auto-detected from the file name and defaults to &apos;ts&apos;.

    -of &lt;outformat&gt;
    --output-format &lt;outformat&gt;
           Specify output format. See -if.

    --input-codec &lt;codec&gt;
           Specify encoding for QM and PO input files. Default is &apos;Latin1&apos;
           for QM and &apos;UTF-8&apos; for PO files. UTF-8 is always tried as well for
           QM, corresponding to the possible use of the trUtf8() function.

    --output-codec &lt;codec&gt;
           Specify encoding for PO output files. Default is &apos;UTF-8&apos;.

    --drop-tags &lt;regexp&gt;
           Drop named extra tags when writing TS or XLIFF files.
           May be specified repeatedly.

    --drop-translations
           Drop existing translations and reset the status to &apos;unfinished&apos;.
           Note: this implies --no-obsolete.

    --source-language &lt;language&gt;[_&lt;region&gt;]
           Specify/override the language of the source strings. Defaults to
           POSIX if not specified and the file does not name it yet.

    --target-language &lt;language&gt;[_&lt;region&gt;]
           Specify/override the language of the translation.
           The target language is guessed from the file name if this option
           is not specified and the file contents name no language yet.

    --no-obsolete
           Drop obsolete messages.

    --no-finished
           Drop finished messages.

    --sort-contexts
           Sort contexts in output TS file alphabetically.

    --locations {absolute|relative|none}
           Override how source code references are saved in TS files.
           Default is absolute.

    --no-ui-lines
           Drop line numbers from references to UI files.

    --verbose
           be a bit more verbose

Long options can be specified with only one leading dash, too.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LRelease</name>
    <message>
        <location filename="../src/tools/lrelease/lrelease.cpp" line="+76"/>
        <source>Usage:
    lrelease [options] ts-files [-qm qm-file]

lrelease is part of Katie&apos;s Linguist tool chain. It can be used as a
stand-alone tool to convert XML-based translations files in the TS
format into the &apos;compiled&apos; QM format used by QTranslator objects.

Options:
    -help
           Display this information and exit.

    -idbased
           Use IDs instead of source strings for message keying.

    -compress
           Compress the QM files.

    -nounfinished
           Do not include unfinished translations.

    -removeidentical
           If the translated text is the same as the source text,
           do not include the message.

    -markuntranslated &lt;prefix&gt;
           If a message has no real translation, use the source text
           prefixed with the given string instead.

    -silent
           Do not explain what is being done.

    -version
           Display the version of lrelease and exit.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+33"/>
        <source>lrelease error: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Updating &apos;%1&apos;...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Removing translations equal to source text in &apos;%1&apos;...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>lrelease error: cannot create &apos;%1&apos;: %2
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>lrelease error: cannot save &apos;%1&apos;: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+68"/>
        <source>lrelease version %1
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LU</name>
    <message>
        <location filename="../src/tools/lupdate/qscript.cpp" line="+1493"/>
        <source>Illegal character</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Unclosed string at end of line</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Illegal escape sequence</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+39"/>
        <source>Illegal unicode escape sequence</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Unclosed comment at end of file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+88"/>
        <source>Illegal syntax for exponential number</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Identifier cannot start with numeric literal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+319"/>
        <source>Unterminated regular expression literal</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+256"/>
        <location line="+23"/>
        <source>//% cannot be used with %1(). Ignoring
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-20"/>
        <source>%1() requires at least two arguments.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>%1(): both arguments must be literal strings.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>%1() requires at least one argument.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>%1(): text to translate must be a literal string.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Discarding unconsumed meta data
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+65"/>
        <source>Expected </source>
        <extracomment>Beginning of the string that contains comma-separated list of expected tokens</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Unexpected character in meta string
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unterminated meta string
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+52"/>
        <source>Cannot open %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LUpdate</name>
    <message>
        <location filename="../src/tools/lupdate/cpp.cpp" line="+628"/>
        <source>Parenthesis/bracket/brace mismatch between #if and #else branches; using #if branch
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Parenthesis/brace mismatch between #if and #else branches; using #if branch
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+27"/>
        <location line="+138"/>
        <source>Unterminated C++ comment
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Unterminated C++ string
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Excess closing brace in C++ code (or abuse of the C++ preprocessor)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Excess closing parenthesis in C++ code (or abuse of the C++ preprocessor)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Excess closing bracket in C++ code (or abuse of the C++ preprocessor)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+430"/>
        <source>Cannot open %1: %2
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+451"/>
        <source>//% cannot be used with tr() / QT_TR_NOOP(). Ignoring
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Qualifying with unknown namespace/class %1::%2
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>tr() cannot be called without context
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <location line="+45"/>
        <source>Class &apos;%1&apos; lacks Q_OBJECT macro
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-16"/>
        <source>It is not recommended to call tr() from within a constructor &apos;%1::%2&apos;
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+38"/>
        <source>//% cannot be used with translate() / QT_TRANSLATE_NOOP(). Ignoring
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+104"/>
        <source>Unexpected character in meta string
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unterminated meta string
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Cannot invoke tr() like this
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Discarding unconsumed meta data
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+40"/>
        <source>Unbalanced opening brace in C++ code (or abuse of the C++ preprocessor)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Unbalanced opening parenthesis in C++ code (or abuse of the C++ preprocessor)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Unbalanced opening bracket in C++ code (or abuse of the C++ preprocessor)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+62"/>
        <location filename="../src/tools/lupdate/ui.cpp" line="+180"/>
        <source>Cannot open %1: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/tools/lupdate/ui.cpp" line="-28"/>
        <source>XML error: Parse error at line %1, column %2 (%3).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Parse error in UI file</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/lupdate/merge.cpp" line="+490"/>
        <source>    Found %n source text(s) (%1 new and %2 already existing)
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+4"/>
        <source>    Removed %n obsolete entries
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+2"/>
        <source>    Kept %n obsolete entries
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+5"/>
        <source>    Number heuristic provided %n translation(s)
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+3"/>
        <source>    Same-text heuristic provided %n translation(s)
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+3"/>
        <source>    Similar-text heuristic provided %n translation(s)
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/lupdate/lupdate.cpp" line="+88"/>
        <source>Usage:
    lupdate [options] [source-file|path|@lst-file]... -ts ts-files|@lst-file

lupdate is part of Katie&apos;s Linguist tool chain. It extracts translatable
messages from Qt UI files, C++ and JavaScript/QtScript source code.
Extracted messages are stored in textual translation source files (typically
Qt TS XML). New and modified messages can be merged into existing TS files.

Options:
    -h
    -help  Display this information and exit.
    -no-obsolete
           Drop all obsolete strings.

    -extensions &lt;ext&gt;[,&lt;ext&gt;]...
           Process files with the given extensions only.
           The extension list must be separated with commas, not with whitespace.
           Default: &apos;%1&apos;.

    -pluralonly
           Only include plural form messages.

    -silent
           Do not explain what is being done.

    -no-sort
           Do not sort contexts in TS files.

    -no-recursive
           Do not recursively scan the following directories.

    -recursive
           Recursively scan the following directories (default).

    -I &lt;includepath&gt; or -I&lt;includepath&gt;
           Additional location to look for include files.
           May be specified multiple times.

    -locations {absolute|relative|none}
           Specify/override how source code references are saved in TS files.
           Default is absolute.

    -no-ui-lines
           Do not record line numbers in references to UI files.

    -disable-heuristic {sametext|similartext|number}
           Disable the named merge heuristic. Can be specified multiple times.

    -source-language &lt;language&gt;[_&lt;region&gt;]
           Specify the language of the source strings for new files.
           Defaults to POSIX if not specified.

    -target-language &lt;language&gt;[_&lt;region&gt;]
           Specify the language of the translations for new files.
           Guessed from the file name if not specified.

    -ts &lt;ts-file&gt;...
           Specify the output file(s). This will override the TRANSLATIONS
           and nullify the CODECFORTR from possibly specified project files.

    -codecfortr &lt;codec&gt;
           Specify the codec assumed for tr() calls. Effective only with -ts.

    -version
           Display the version of lupdate and exit.

    @lst-file
           Read additional file names (one per line) from lst-file.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+74"/>
        <source>lupdate warning: Codec for tr() &apos;%1&apos; disagrees with existing file&apos;s codec &apos;%2&apos;. Expect trouble.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>lupdate warning: Specified target language &apos;%1&apos; disagrees with existing file&apos;s language &apos;%2&apos;. Ignoring.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>lupdate warning: Specified source language &apos;%1&apos; disagrees with existing file&apos;s language &apos;%2&apos;. Ignoring.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Updating &apos;%1&apos;...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Stripping non plural forms in &apos;%1&apos;...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+97"/>
        <source>The option -target-language requires a parameter.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The option -source-language requires a parameter.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The option -disable-heuristic requires a parameter.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Invalid heuristic name passed to -disable-heuristic.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>The option -locations requires a parameter.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Invalid parameter passed to -locations.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+26"/>
        <source>The -codecfortr option should be followed by a codec name.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>The -extensions option should be followed by an extension list.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>The -I option should be followed by a path.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Unrecognized option &apos;%1&apos;.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>lupdate error: List file &apos;%1&apos; is not readable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>lupdate warning: For some reason, &apos;%1&apos; is not writable.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>lupdate error: File &apos;%1&apos; has no recognized extension.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>lupdate error: File &apos;%1&apos; does not exist.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Scanning directory &apos;%1&apos;...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+49"/>
        <source>lupdate warning: -target-language usually only makes sense with exactly one TS file.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>lupdate warning: -codecfortr has no effect without -ts.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>lupdate warning: no TS files specified. Only diagnostics will be produced.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <source></source>
        <comment>This is the application&apos;s main window.</comment>
        <translation></translation>
    </message>
    <message>
        <location filename="../src/tools/qtconfig/mainwindow.ui"/>
        <source>Qt Configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Appearance</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>GUI Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Select GUI &amp;Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Select &amp;Palette:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Active Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Inactive Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Disabled Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Build Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Button Background:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Window Back&amp;ground:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Tune Palette...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Fonts</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Default Font</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Style:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Point Size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>F&amp;amily:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Sample Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Font Substitution</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>S&amp;elect or Enter a Family:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Current Substitutions:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Select s&amp;ubstitute Family:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Interface</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Feel Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source> ms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Double Click Interval:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>No blinking</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Cursor Flash Time:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source> lines</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Wheel &amp;Scroll Lines:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Resolve symlinks in URLs</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>GUI Effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Enable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Alt+E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Menu Effect:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>C&amp;omboBox Effect:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;ToolTip Effect:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Tool&amp;Box Effect:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Disable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Animate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Fade</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Global Strut</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Minimum &amp;Width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Minimum Hei&amp;ght:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source> pixels</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enhanced support for languages written right-to-left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Printer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Enable Font embedding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Font Paths</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Browse...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Press the &lt;b&gt;Browse&lt;/b&gt; button or enter a directory and press Enter to add them to the list.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>E&amp;xit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Exit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Ctrl+Q</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;About</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>About</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/tools/qtconfig/mainwindow.cpp" line="+75"/>
        <source>&lt;p&gt;&lt;b&gt;&lt;font size+=2&gt;Appearance&lt;/font&gt;&lt;/b&gt;&lt;/p&gt;&lt;hr&gt;&lt;p&gt;Use this tab to customize the appearance of your Qt applications.&lt;/p&gt;&lt;p&gt;You can select the default GUI Style from the drop down list and customize the colors.&lt;/p&gt;&lt;p&gt;Any GUI Style plugins in your plugin path will automatically be added to the list of built-in Qt styles. (See the Library Paths tab for information on adding new plugin paths.)&lt;/p&gt;&lt;p&gt;When you choose 3-D Effects and Window Background colors, the Qt Configuration program will automatically generate a palette for you. To customize colors further, press the Tune Palette button to open the advanced palette editor.&lt;p&gt;The Preview Window shows what the selected Style and colors look like.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>&lt;p&gt;&lt;b&gt;&lt;font size+=2&gt;Fonts&lt;/font&gt;&lt;/b&gt;&lt;/p&gt;&lt;hr&gt;&lt;p&gt;Use this tab to select the default font for your Qt applications. The selected font is shown (initially as &apos;Sample Text&apos;) in the line edit below the Family, Style and Point Size drop down lists.&lt;/p&gt;&lt;p&gt;Qt has a powerful font substitution feature that allows you to specify a list of substitute fonts.  Substitute fonts are used when a font cannot be loaded, or if the specified font doesn&apos;t have a particular character.&lt;p&gt;For example, if you select the font Lucida, which doesn&apos;t have Korean characters, but need to show some Korean text using the Mincho font family you can do so by adding Mincho to the list. Once Mincho is added, any Korean characters that are not found in the Lucida font will be taken from the Mincho font.  Because the font substitutions are lists, you can also select multiple families, such as Song Ti (for use with Chinese text).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>&lt;p&gt;&lt;b&gt;&lt;font size+=2&gt;Interface&lt;/font&gt;&lt;/b&gt;&lt;/p&gt;&lt;hr&gt;&lt;p&gt;Use this tab to customize the feel of your Qt applications.&lt;/p&gt;&lt;p&gt;If the Resolve Symlinks checkbox is checked Qt will follow symlinks when handling URLs. For example, in the file dialog, if this setting is turned on and /usr/tmp is a symlink to /var/tmp, entering the /usr/tmp directory will cause the file dialog to change to /var/tmp.  With this setting turned off, symlinks are not resolved or followed.&lt;/p&gt;&lt;p&gt;The Global Strut setting is useful for people who require a minimum size for all widgets (e.g. when using a touch panel or for users who are visually impaired).  Leaving the Global Strut width and height at 0 will disable the Global Strut feature.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>&lt;p&gt;&lt;b&gt;&lt;font size+=2&gt;Printer&lt;/font&gt;&lt;/b&gt;&lt;/p&gt;&lt;hr&gt;&lt;p&gt;Use this tab to configure the way Qt generates output for the printer.You can specify if Qt should try to embed fonts into its generated output.If you enable font embedding, the resulting postscript will be more portable and will more accurately reflect the visual output on the screen; however the resulting postscript file size will be bigger.&lt;p&gt;When using font embedding you can select additional directories where Qt should search for embeddable font files.  By default, the X server font path is used.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+84"/>
        <source>Desktop Settings (Default)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Choose style and palette based on your desktop settings.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Unknown</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+148"/>
        <source>No changes to be saved.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Saving changes...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+89"/>
        <source>Saved changes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+267"/>
        <source>Select a Directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>&lt;h3&gt;%1&lt;/h3&gt;&lt;br/&gt;Version %2&lt;br/&gt;&lt;br/&gt;Copyright (C) 2015 The Qt Company Ltd.&lt;br/&gt;&lt;br/&gt;Copyright (C) 2016-2019 Ivailo Monev</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <location line="+1"/>
        <location line="+7"/>
        <source>Katie Configuration</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Save Changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Save changes to settings?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/tools/qtconfig/mainwindow.ui"/>
        <source>About &amp;Katie</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>About Katie</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MainWindowBase</name>
    <message>
        <location filename="../src/tools/designer/mainwindow.cpp" line="+119"/>
        <source>Main</source>
        <extracomment>Not currently used (main tool bar)</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Tools</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Katie Designer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>MessageEditor</name>
    <message>
        <source></source>
        <comment>This is the right panel of the main window.</comment>
        <translation></translation>
    </message>
</context>
<context>
    <name>NewForm</name>
    <message>
        <location filename="../src/tools/designer/newform.cpp" line="+74"/>
        <source>Show this Dialog on Startup</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>C&amp;reate</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Recent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>New Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+29"/>
        <source>&amp;Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&amp;Open...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&amp;Recent Forms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+64"/>
        <source>Read error</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>A temporary form file could not be created in %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The temporary form file %1 could not be written.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PaletteEditorAdvanced</name>
    <message>
        <location filename="../src/tools/qtconfig/paletteeditoradvanced.ui"/>
        <source>Tune Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Select &amp;Palette:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Active Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Inactive Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Disabled Palette</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Auto</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Build inactive palette from active</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Build disabled palette from active</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Central color &amp;roles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Choose central color role</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Select a color role.&lt;/b&gt;&lt;p&gt;Available central roles are: &lt;ul&gt; &lt;li&gt;Window - general background color.&lt;/li&gt; &lt;li&gt;WindowText - general foreground color. &lt;/li&gt; &lt;li&gt;Base - used as background color for e.g. text entry widgets, usually white or another light color. &lt;/li&gt; &lt;li&gt;Text - the foreground color used with Base. Usually this is the same as WindowText, in what case it must provide good contrast both with Window and Base. &lt;/li&gt; &lt;li&gt;Button - general button background color, where buttons need a background different from Window, as in the Macintosh style. &lt;/li&gt; &lt;li&gt;ButtonText - a foreground color used with the Button color. &lt;/li&gt; &lt;li&gt;Highlight - a color to indicate a selected or highlighted item. &lt;/li&gt; &lt;li&gt;HighlightedText - a text color that contrasts to Highlight. &lt;/li&gt; &lt;li&gt;BrightText - a text color that is very different from WindowText and contrasts well with e.g. black. &lt;/li&gt; &lt;/ul&gt; &lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>WindowText</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Base</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>AlternateBase</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>ToolTipBase</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>ToolTipText</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Button</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>ButtonText</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>BrightText</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Highlight</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>HighlightedText</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Link</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>LinkVisited</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Select Color:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>3-D shadow &amp;effects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Generate shadings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Check to let 3D-effect colors be calculated from button-color.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Build &amp;from button color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Choose 3D-effect color role</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;b&gt;Select a color role.&lt;/b&gt;&lt;p&gt;Available effect roles are: &lt;ul&gt; &lt;li&gt;Light - lighter than Button color. &lt;/li&gt; &lt;li&gt;Midlight - between Button and Light. &lt;/li&gt; &lt;li&gt;Mid - between Button and Dark. &lt;/li&gt; &lt;li&gt;Dark - darker than Button. &lt;/li&gt; &lt;li&gt;Shadow - a very dark color. &lt;/li&gt; &lt;/ul&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Light</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Midlight</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Mid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Dark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Shadow</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Select Co&amp;lor:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/tools/qtconfig/paletteeditoradvanced.cpp" line="+54"/>
        <location line="+6"/>
        <source>Choose a color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-5"/>
        <source>Choose a color for the selected central color role.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Choose a color for the selected effect color role.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PhraseBookBox</name>
    <message>
        <source></source>
        <comment>Go to Phrase &gt; Edit Phrase Book... The dialog that pops up is a PhraseBookBox.</comment>
        <translation></translation>
    </message>
</context>
<context>
    <name>PreferencesDialog</name>
    <message>
        <location filename="../src/tools/designer/preferencesdialog.ui"/>
        <source>Preferences</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PreviewFrame</name>
    <message>
        <location filename="../src/tools/qtconfig/previewframe.cpp" line="+83"/>
        <source>Desktop settings will only take effect after an application restart.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>PreviewWidget</name>
    <message>
        <location filename="../src/tools/qtconfig/previewwidget.ui"/>
        <source>Preview Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>GroupBox</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>RadioButton1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>RadioButton2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>RadioButton3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>GroupBox2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>CheckBox1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>CheckBox2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>LineEdit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>ComboBox</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>PushButton</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;p&gt;&lt;a href=&quot;http://qt.nokia.com&quot;&gt;http://qt.nokia.com&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;&lt;a href=&quot;http://www.kde.org&quot;&gt;http://www.kde.org&lt;/a&gt;&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QDBusViewer</name>
    <message>
        <location filename="../src/tools/qdbusviewer/qdbusviewer.cpp" line="+83"/>
        <source>Search...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>&amp;Refresh</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+116"/>
        <source>Arguments</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Please enter the value of the property %1 (type %2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Unable to marshall</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Value conversion failed, unable to set property</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Unable to find method</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to find method %1 on path %2 in interface %3</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Please enter parameters for the method &quot;%1&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+50"/>
        <source>&amp;Connect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&amp;Call</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&amp;Set value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&amp;Get value</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Unable to connect to service %1, path %2, interface %3, signal %4</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+122"/>
        <location line="+1"/>
        <source>D-Bus Viewer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QDesigner</name>
    <message>
        <location filename="../src/tools/designer/qdesigner.cpp" line="+132"/>
        <source>%1 - warning</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QDesignerActions</name>
    <message>
        <location filename="../src/tools/designer/qdesigner_actions.cpp" line="+130"/>
        <source>Saved %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+26"/>
        <source>%1 already exists.
Do you want to replace it?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Edit Widgets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;New...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;Open...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Save &amp;As...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Save A&amp;ll</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Save As &amp;Template...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+854"/>
        <source>&amp;Close</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-853"/>
        <source>Save &amp;Image...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;Print...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;Quit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>View &amp;Code...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;Minimize</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Bring All to Front</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Preferences...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Additional Fonts...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+72"/>
        <source>CTRL+SHIFT+S</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+126"/>
        <source>CTRL+R</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>CTRL+M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+31"/>
        <source>About Plugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <location line="+571"/>
        <source>About Katie Designer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-565"/>
        <source>About Katie</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Clear &amp;Menu</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>&amp;Recent Forms</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+77"/>
        <location line="+197"/>
        <source>Open Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-196"/>
        <location line="+37"/>
        <location line="+160"/>
        <source>Designer UI files (*.%1);;All Files (*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-160"/>
        <location line="+248"/>
        <source>Save Form As</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-161"/>
        <source>Designer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Feature not implemented yet!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Code generation failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Read error</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>%1
Do you want to update the file location or generate a new form?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&amp;Update</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;New Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+77"/>
        <location line="+40"/>
        <source>Save Form?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-39"/>
        <source>Could not open file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>The file %1 could not be opened.
Reason: %2
Would you like to retry or select a different file?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Select New File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Could not write file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>It was not possible to write the entire file %1 to disk.
Reason:%2
Would you like to retry?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+153"/>
        <source>&amp;Close Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+41"/>
        <location line="+23"/>
        <source>The backup file %1 could not be written.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+107"/>
        <source>The backup directory %1 could not be created.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The temporary backup directory %1 could not be created.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Preview failed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Image files (*.%1)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <location line="+17"/>
        <source>Save Image</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-4"/>
        <source>Saved image %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file %1 could not be written.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Please close all forms to enable the loading of additional fonts.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+51"/>
        <source>Printed %1.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QDesignerAppearanceOptionsPage</name>
    <message>
        <location filename="../src/tools/designer/qdesigner_appearanceoptions.cpp" line="+138"/>
        <source>Appearance</source>
        <extracomment>Tab in preferences dialog</extracomment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QDesignerAppearanceOptionsWidget</name>
    <message>
        <location line="-53"/>
        <source>Docked Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Multiple Top-Level Windows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Toolwindow Font</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QDesignerFormWindow</name>
    <message>
        <location filename="../src/tools/designer/qdesigner_formwindow.cpp" line="+218"/>
        <source>%1 - %2[*]</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Save Form?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Do you want to save the changes to this document before closing?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>If you don&apos;t save, your changes will be lost.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QDesignerToolWindow</name>
    <message>
        <location filename="../src/tools/designer/qdesigner_toolwindow.cpp" line="+190"/>
        <source>Property Editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+54"/>
        <source>Action Editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Object Inspector</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Resource Browser</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Signal/Slot Editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Widget Box</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QDesignerWorkbench</name>
    <message>
        <location filename="../src/tools/designer/qdesigner_workbench.cpp" line="+199"/>
        <source>&amp;File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&amp;Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>F&amp;orm</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Preview in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&amp;View</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&amp;Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&amp;Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&amp;Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Toolbars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+166"/>
        <source>Widget Box</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+282"/>
        <source>Save Forms?</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location line="+1"/>
        <source>There are %n forms with unsaved changes. Do you want to review these changes before quitting?</source>
        <translation type="unfinished">
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <location line="+3"/>
        <source>If you do not review your documents, all your changes will be lost.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Discard Changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Review Changes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+95"/>
        <source>Backup Information</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The last session of Designer was not terminated correctly. Backup files were left behind. Do you want to load them?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+110"/>
        <source>The file &lt;b&gt;%1&lt;/b&gt; could not be opened.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+46"/>
        <source>The file &lt;b&gt;%1&lt;/b&gt; is not a valid Designer UI file.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/tools/lupdate/lupdate.cpp" line="-139"/>
        <source>lupdate version %1
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/tools/qdbusviewer/qdbusviewermain.cpp" line="+66"/>
        <source>Session Bus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>System Bus</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>&amp;File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;Quit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&amp;Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&amp;About</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>About &amp;Katie</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QtDBusViewer</name>
    <message>
        <location line="-19"/>
        <source>Katie D-Bus Viewer</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QtToolBarDialog</name>
    <message>
        <location filename="../src/tools/designer/qttoolbardialog/qttoolbardialog.ui"/>
        <source>Customize Toolbars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Actions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Toolbars</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add new toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>New</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Remove selected toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Remove</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Rename toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Rename</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Move action up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Up</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Remove action from toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&lt;-</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Add action to toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>-&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Move action down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Down</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>Current Toolbar Actions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/tools/designer/qttoolbardialog/qttoolbardialog.cpp" line="+1242"/>
        <source>Custom Toolbar</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+544"/>
        <source>&lt; S E P A R A T O R &gt;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SaveFormAsTemplate</name>
    <message>
        <location filename="../src/tools/designer/saveformastemplate.ui"/>
        <source>Save Form As Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location/>
        <source>&amp;Category:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../src/tools/designer/saveformastemplate.cpp" line="+72"/>
        <source>Add path...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Template Exists</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A template with the name %1 already exists.
Do you want overwrite the template?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Overwrite Template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Open Error</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>There was an error opening template %1 for writing. Reason: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Write Error</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>There was an error writing the template %1 to disk. Reason: %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Pick a directory to save templates in</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ToolBarManager</name>
    <message>
        <location filename="../src/tools/designer/mainwindow.cpp" line="+89"/>
        <source>Configure Toolbars...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Window</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Help</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Dock views</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Tools</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Form</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Toolbars</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>VersionDialog</name>
    <message>
        <location filename="../src/tools/designer/versiondialog.cpp" line="+166"/>
        <source>&lt;h3&gt;%1&lt;/h3&gt;&lt;br/&gt;&lt;br/&gt;Version %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Katie Designer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;br/&gt;Katie Designer is a graphical user interface designer for Qt applications.&lt;br/&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1&lt;br/&gt;Copyright (C) 2015 The Qt Company Ltd.&lt;br/&gt;Copyright (C) 2016-2019 Ivailo Monev</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
