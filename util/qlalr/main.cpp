/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the utils of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

#include <cstdlib>

#include "lalr.h"
#include "dotgraph.h"
#include "parsetable.h"
#include "cppgenerator.h"
#include "recognizer.h"

#define QLALR_NO_DEBUG_TABLE
#define QLALR_NO_DEBUG_DOT

QT_USE_NAMESPACE

static void help_me ()
{
  qerr << "Usage: qlalr [options] [input file name]" << endl
       << endl
       << "  --help, -h\t\tdisplay this help and exit" << endl
       << "  --verbose, -v\t\tverbose output" << endl
       << "  --no-debug\t\tno debug information" << endl
       << "  --no-lines\t\tno #line directives" << endl
       << "  --dot\t\t\tgenerate a graph" << endl
       << "  --qt\t\tadd the Qt copyright header and Qt-specific types and macros" << endl
       << endl;
  exit (0);
}

int main (int argc, char *argv[])
{
  QCoreApplication app (argc, argv);

  bool generate_dot = false;
  bool generate_report = false;
  bool no_lines = false;
  bool debug_info = true;
  bool troll_copyright = false;
  QString file_name;

  QStringList args = app.arguments ();
  args.removeFirst ();

  foreach (const QString &arg, args)
    {
      if (arg == QLatin1String ("-h") || arg == QLatin1String ("--help"))
        help_me ();

      else if (arg == QLatin1String ("-v") || arg == QLatin1String ("--verbose"))
        generate_report = true;

      else if (arg == QLatin1String ("--dot"))
        generate_dot = true;

      else if (arg == QLatin1String ("--no-lines"))
        no_lines = true;

      else if (arg == QLatin1String ("--no-debug"))
        debug_info = false;

      else if (arg == QLatin1String ("--qt"))
        troll_copyright = true;

      else if (file_name.isEmpty ())
	file_name = arg;

      else
        qerr << "*** Warning. Ignore argument `" << arg << "'" << endl;
    }

  if (file_name.isEmpty ())
    {
      help_me ();
      exit (EXIT_SUCCESS);
    }

  Grammar grammar;
  Recognizer p (&grammar, no_lines);

  if (! p.parse (file_name))
    exit (EXIT_FAILURE);

  if (grammar.rules.size() == 0)
    {
      qerr << "*** Fatal. No rules!" << endl;
      exit (EXIT_FAILURE);
    }

  else if (grammar.start == grammar.names.end ())
    {
      qerr << "*** Fatal. No start symbol!" << endl;
      exit (EXIT_FAILURE);
    }

  grammar.buildExtendedGrammar ();
  grammar.buildRuleMap ();

  Automaton aut (&grammar);
  aut.build ();

  CppGenerator gen (p, grammar, aut, generate_report);
  gen.setDebugInfo (debug_info);
  gen.setCopyright (troll_copyright);
  gen ();

  if (generate_dot)
    {
      DotGraph genDotFile (qout);
      genDotFile (&aut);
    }

  else if (generate_report)
    {
      ParseTable genParseTable (qout);
      genParseTable(&aut);
    }

  return EXIT_SUCCESS;
}

QString Recognizer::expand (const QString &text) const
{
  QString code = text;

  if (_M_grammar->start != _M_grammar->names.end ())
    {
      code = code.replace (QLatin1String("$start_id"), QString::number (std::distance (_M_grammar->names.begin (), _M_grammar->start)));
      code = code.replace (QLatin1String("$start"), *_M_grammar->start);
    }

  code = code.replace (QLatin1String("$header"), _M_grammar->table_name.toLower () + QLatin1String("_p.h"));

  code = code.replace (QLatin1String("$table"), _M_grammar->table_name);
  code = code.replace (QLatin1String("$parser"), _M_grammar->table_name);

  if (_M_current_rule != _M_grammar->rules.end ())
    {
      code = code.replace (QLatin1String("$rule_number"), QString::number (std::distance (_M_grammar->rules.begin (), _M_current_rule)));
      code = code.replace (QLatin1String("$rule"), *_M_current_rule->lhs);
    }

  return code;
}
