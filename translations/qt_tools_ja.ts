<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="ja">
<context>
    <name>FindDialog</name>
    <message>
        <source></source>
        <comment>Choose Edit|Find from the menu bar or press Ctrl+F to pop up the Find dialog</comment>
        <translation></translation>
    </message>
</context>
<context>
    <name>LConvert</name>
    <message>
        <location filename="../src/tools/lconvert/lconvert.cpp" line="+68"/>
        <source>
Usage:
    lconvert [options] &lt;infile&gt; [&lt;infile&gt;...]

lconvert is part of Qt&apos;s Linguist tool chain. It can be used as a
stand-alone tool to convert and filter translation data files.
The following file formats are supported:

%1
If multiple input files are specified, they are merged with
translations from later files taking precedence.

Options:
    -h
    --help  Display this information and exit.

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

Return value:
    0 on success
    1 on command line parse failures
    2 on read failures
    3 on write failures
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LRelease</name>
    <message>
        <location filename="../src/tools/lrelease/lrelease.cpp" line="+95"/>
        <source>Usage:
    lrelease [options] ts-files [-qm qm-file]

lrelease is part of Qt&apos;s Linguist tool chain. It can be used as a
stand-alone tool to convert XML-based translations files in the TS
format into the &apos;compiled&apos; QM format used by QTranslator objects.

Options:
    -help  Display this information and exit
    -idbased
           Use IDs instead of source strings for message keying
    -compress
           Compress the QM files
    -nounfinished
           Do not include unfinished translations
    -removeidentical
           If the translated text is the same as
           the source text, do not include the message
    -markuntranslated &lt;prefix&gt;
           If a message has no real translation, use the source text
           prefixed with the given string instead
    -silent
           Do not explain what is being done
    -version
           Display the version of lrelease and exit
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+32"/>
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
        <location line="+87"/>
        <source>lrelease version %1
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>LUpdate</name>
    <message>
        <location filename="../src/tools/lupdate/cpp.cpp" line="+638"/>
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
        <location line="+144"/>
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
        <location line="+55"/>
        <source>//= cannot be used with qtTrId() / QT_TRID_NOOP(). Ignoring
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+67"/>
        <location filename="../src/tools/lupdate/qscript.cpp" line="+2541"/>
        <source>Unexpected character in meta string
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <location filename="../src/tools/lupdate/qscript.cpp" line="+6"/>
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
        <location filename="../src/tools/lupdate/qscript.cpp" line="-126"/>
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
        <location filename="../src/tools/lupdate/ui.cpp" line="+188"/>
        <location filename="../src/tools/lupdate/qscript.cpp" line="+178"/>
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
    <message>
        <location filename="../src/tools/lupdate/qscript.cpp" line="-1089"/>
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
        <location line="+21"/>
        <source>%1() requires at least one argument.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-18"/>
        <source>%1(): text to translate must be a literal string.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>//= cannot be used with %1(). Ignoring
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>%1(): identifier must be a literal string.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+133"/>
        <source>Expected </source>
        <extracomment>Beginning of the string that contains comma-separated list of expected tokens</extracomment>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../src/tools/lupdate/merge.cpp" line="+494"/>
        <source>    Found %n source text(s) (%1 new and %2 already existing)
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+4"/>
        <source>    Removed %n obsolete entries
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+2"/>
        <source>    Kept %n obsolete entries
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+5"/>
        <source>    Number heuristic provided %n translation(s)
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+3"/>
        <source>    Same-text heuristic provided %n translation(s)
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+3"/>
        <source>    Similar-text heuristic provided %n translation(s)
</source>
        <translation type="unfinished">
            <numerusform></numerusform>
        </translation>
    </message>
    <message>
        <location filename="../src/tools/lupdate/lupdate.cpp" line="+90"/>
        <source>Usage:
    lupdate [options] [source-file|path|@lst-file]... -ts ts-files|@lst-file

lupdate is part of Qt&apos;s Linguist tool chain. It extracts translatable
messages from Qt UI files, C++ and JavaScript/QtScript source code.
Extracted messages are stored in textual translation source files (typically
Qt TS XML). New and modified messages can be merged into existing TS files.

Options:
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
        <location line="+73"/>
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
    <name>MsgEdit</name>
    <message>
        <source></source>
        <comment>This is the right panel of the main window.</comment>
        <translation></translation>
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
    <name>QCollectionGenerator</name>
    <message>
        <location filename="../src/tools/qcollectiongenerator/qcollectiongenerator.cpp" line="+137"/>
        <source>Unknown token at line %1.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Unknown token at line %1. Expected &quot;QtHelpCollectionProject&quot;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Missing end tags.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+145"/>
        <source>Missing input or output file for help file generation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+59"/>
        <source>Missing output file name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Qt Collection Generator version 1.0 (Qt %1)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Missing collection config file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>
Usage:

qcollectiongenerator &lt;collection-config-file&gt; [options]

  -o &lt;collection-file&gt;   Generates a collection file
                         called &lt;collection-file&gt;. If
                         this option is not specified
                         a default name will be used.
  -v                     Displays the version of
                         qcollectiongenerator.

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Could not open %1.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Reading collection config file...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Collection config file error: %1
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Generating help for %1...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Creating collection file...
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The file %1 cannot be overwritten.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+61"/>
        <location line="+21"/>
        <location line="+22"/>
        <source>Cannot open %1.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Cannot open referenced image file %1.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QHelpGenerator</name>
    <message>
        <location filename="../src/tools/qhelpgenerator/qhelpgenerator.cpp" line="+90"/>
        <source>Missing output file name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Qt Help Generator version 1.0 (Qt %1)
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Missing Qt help project file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>
Usage:

qhelpgenerator &lt;help-project-file&gt; [options]

  -o &lt;compressed-file&gt;   Generates a Qt compressed help
                         file called &lt;compressed-file&gt;.
                         If this option is not specified
                         a default name will be used.
  -c                     Checks whether all links in HTML files
                         point to files in this help project.
  -v                     Displays the version of 
                         qhelpgenerator.

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Could not open %1.
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Could not create output directory: %1
</source>
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
</context>
<context>
    <name>TrWindow</name>
    <message>
        <source></source>
        <comment>This is the application&apos;s main window.</comment>
        <translation></translation>
    </message>
</context>
</TS>
