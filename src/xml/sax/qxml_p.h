/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtXml module of the Katie Toolkit.
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

#ifndef QXML_P_H
#define QXML_P_H

#include <QtCore/qstack.h>

QT_BEGIN_NAMESPACE

class QXmlSimpleReaderPrivate
{
public:
    ~QXmlSimpleReaderPrivate();
private:
    QXmlSimpleReaderPrivate(QXmlSimpleReader *reader);

    // functions
    void initIncrementalParsing();

    // used to determine if elements are correctly nested
    QStack<QString> tags;

    // used by parseReference() and parsePEReference()
    enum EntityRecognitionContext { InContent, InAttributeValue, InEntityValue, InDTD };

    // used for entity declarations
    struct ExternParameterEntity
    {
        ExternParameterEntity() {}
        ExternParameterEntity(const QString &p, const QString &s)
            : publicId(p), systemId(s) {}
        QString publicId;
        QString systemId;
    };
    struct ExternEntity
    {
        ExternEntity() {}
        ExternEntity(const QString &p, const QString &s, const QString &n)
            : publicId(p), systemId(s), notation(n) {}
        QString publicId;
        QString systemId;
        QString notation;
    };
    QMap<QString,ExternParameterEntity> externParameterEntities;
    QMap<QString,QString> parameterEntities;
    QMap<QString,ExternEntity> externEntities;
    QMap<QString,QString> entities;

    // used for parsing of entity references
    struct XmlRef {
        XmlRef()
            : index(0) {}
        XmlRef(const QString &_name, const QString &_value)
            : name(_name), value(_value), index(0) {}
        bool isEmpty() const { return index == value.length(); }
        QChar next() { return value.at(index++); }
        QString name;
        QString value;
        int index;
    };
    QStack<XmlRef> xmlRefStack;

    // used for standalone declaration
    enum Standalone { Yes, No, Unknown };

    QString doctype; // only used for the doctype
    QString xmlVersion; // only used to store the version information
    QString encoding; // only used to store the encoding
    Standalone standalone; // used to store the value of the standalone declaration

    QString publicId; // used by parseExternalID() to store the public ID
    QString systemId; // used by parseExternalID() to store the system ID

    // Since publicId/systemId is used as temporary variables by parseExternalID(), it
    // might overwrite the PUBLIC/SYSTEM for the document we're parsing. In effect, we would
    // possibly send off an QXmlParseException that has the PUBLIC/SYSTEM of a entity declaration
    // instead of those of the current document.
    // Hence we have these two variables for storing the document's data.
    QString thisPublicId;
    QString thisSystemId;

    QString attDeclEName; // use by parseAttlistDecl()
    QString attDeclAName; // use by parseAttlistDecl()

    // flags for some features support
    bool useNamespaces;
    bool useNamespacePrefixes;
    bool reportWhitespaceCharData;
    bool reportEntities;

    // used to build the attribute list
    QXmlAttributes attList;

    // used in QXmlSimpleReader::parseContent() to decide whether character
    // data was read
    bool contentCharDataRead;
    // Hack for letting QDom know where the skipped entity occurred
    bool skipped_entity_in_content;

    // helper classes
    QXmlLocator* locator;
    QXmlNamespaceSupport namespaceSupport;

    // error string
    QString error;

    // arguments for parse functions (this is needed to allow incremental
    // parsing)
    bool parsePI_xmldecl;
    bool parseName_useRef;
    bool parseReference_charDataRead;
    EntityRecognitionContext parseReference_context;
    bool parseExternalID_allowPublicID;
    EntityRecognitionContext parsePEReference_context;
    QString parseString_s;

    // for incremental parsing
    struct ParseState {
        typedef bool (QXmlSimpleReaderPrivate::*ParseFunction)();
        ParseFunction function;
        int state;
    };
    QStack<ParseState> *parseStack;

    // used in parseProlog()
    bool xmldecl_possible;
    bool doctype_read;

    // used in parseDoctype()
    bool startDTDwasReported;

    // used in parseString()
    signed char Done;


    // variables
    QXmlContentHandler *contentHnd;
    QXmlErrorHandler   *errorHnd;
    QXmlDTDHandler     *dtdHnd;
    QXmlEntityResolver *entityRes;
    QXmlLexicalHandler *lexicalHnd;
    QXmlDeclHandler    *declHnd;

    QXmlInputSource *inputSource;

    QChar c; // the character at reading position
    int   lineNr; // number of line
    int   columnNr; // position in line

    QChar   nameArray[256]; // only used for names
    QString nameValue; // only used for names
    int     nameArrayPos;
    int     nameValueLen;
    QChar   refArray[256]; // only used for references
    QString refValue; // only used for references
    int     refArrayPos;
    int     refValueLen;
    QChar   stringArray[256]; // used for any other strings that are parsed
    QString stringValue; // used for any other strings that are parsed
    int     stringArrayPos;
    int     stringValueLen;

    QHash<QString, int> literalEntitySizes;
    // The entity at (QMap<QString,) referenced the entities at (QMap<QString,) (int>) times.
    QHash<QString, QHash<QString, int> > referencesToOtherEntities;
    QHash<QString, int> expandedSizes;
    // The limit to the amount of times the DTD parsing functions can be called
    // for the DTD currently being parsed.
    static const int dtdRecursionLimit = 2;
    // The maximum amount of characters an entity value may contain, after expansion.
    static const int entityCharacterLimit = 1024;

    const QString &string();
    void stringClear();
    void stringAddC(QChar);
    inline void stringAddC() { stringAddC(c); }
    const QString &name();
    void nameClear();
    void nameAddC(QChar);
    inline void nameAddC() { nameAddC(c); }
    const QString &ref();
    void refClear();
    void refAddC(QChar);
    inline void refAddC() { refAddC(c); }

    // private functions
    bool eat_ws();
    bool next_eat_ws();

    void QT_FASTCALL next();
    bool atEnd();

    void init(const QXmlInputSource* i);
    void initData();

    bool entityExist(const QString&) const;

    bool parseBeginOrContinue(int state, bool incremental);

    bool parseProlog();
    bool parseElement();
    bool processElementEmptyTag();
    bool processElementETagBegin2();
    bool processElementAttribute();
    bool parseMisc();
    bool parseContent();

    bool parsePI();
    bool parseDoctype();
    bool parseComment();

    bool parseName();
    bool parseNmtoken();
    bool parseAttribute();
    bool parseReference();
    bool processReference();

    bool parseExternalID();
    bool parsePEReference();
    bool parseMarkupdecl();
    bool parseAttlistDecl();
    bool parseAttType();
    bool parseAttValue();
    bool parseElementDecl();
    bool parseNotationDecl();
    bool parseChoiceSeq();
    bool parseEntityDecl();
    bool parseEntityValue();

    bool parseString();

    bool insertXmlRef(const QString&, const QString&, bool);

    bool reportEndEntities();
    void reportParseError(const QString& error);

    typedef bool (QXmlSimpleReaderPrivate::*ParseFunction) ();
    void unexpectedEof(ParseFunction where, int state);
    void parseFailed(ParseFunction where, int state);
    void pushParseState(ParseFunction function, int state);
    bool isExpandedEntityValueTooLarge(QString *errorMessage);

    friend class QXmlSimpleReader;
    friend class QXmlSimpleReaderLocator;
    friend class QDomHandler;
};

QT_END_NAMESPACE

#endif // QXML_P_H
