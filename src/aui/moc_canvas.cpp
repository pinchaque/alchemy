/****************************************************************************
** FigureEditor meta object code from reading C++ file 'canvas.h'
**
** Created: Sat May 15 13:22:14 2004
**      by: The Qt MOC ($Id: moc_canvas.cpp,v 1.1 2004-07-11 18:44:43 chuck Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "canvas.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FigureEditor::className() const
{
    return "FigureEditor";
}

QMetaObject *FigureEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FigureEditor( "FigureEditor", &FigureEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FigureEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FigureEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FigureEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FigureEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FigureEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QCanvasView::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"status", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "status(const QString&)", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"FigureEditor", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FigureEditor.setMetaObject( metaObj );
    return metaObj;
}

void* FigureEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FigureEditor" ) )
	return this;
    return QCanvasView::qt_cast( clname );
}

// SIGNAL status
void FigureEditor::status( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool FigureEditor::qt_invoke( int _id, QUObject* _o )
{
    return QCanvasView::qt_invoke(_id,_o);
}

bool FigureEditor::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: status((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QCanvasView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool FigureEditor::qt_property( int id, int f, QVariant* v)
{
    return QCanvasView::qt_property( id, f, v);
}

bool FigureEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *Main::className() const
{
    return "Main";
}

QMetaObject *Main::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Main( "Main", &Main::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Main::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Main", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Main::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Main", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Main::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"help", 0, 0 };
    static const QUMethod slot_1 = {"aboutQt", 0, 0 };
    static const QUMethod slot_2 = {"newView", 0, 0 };
    static const QUMethod slot_3 = {"clear", 0, 0 };
    static const QUMethod slot_4 = {"init", 0, 0 };
    static const QUMethod slot_5 = {"addSprite", 0, 0 };
    static const QUMethod slot_6 = {"addCircle", 0, 0 };
    static const QUMethod slot_7 = {"addHexagon", 0, 0 };
    static const QUMethod slot_8 = {"addPolygon", 0, 0 };
    static const QUMethod slot_9 = {"addSpline", 0, 0 };
    static const QUMethod slot_10 = {"addText", 0, 0 };
    static const QUMethod slot_11 = {"addLine", 0, 0 };
    static const QUMethod slot_12 = {"addRectangle", 0, 0 };
    static const QUMethod slot_13 = {"addMesh", 0, 0 };
    static const QUMethod slot_14 = {"addLogo", 0, 0 };
    static const QUMethod slot_15 = {"addButterfly", 0, 0 };
    static const QUMethod slot_16 = {"enlarge", 0, 0 };
    static const QUMethod slot_17 = {"shrink", 0, 0 };
    static const QUMethod slot_18 = {"rotateClockwise", 0, 0 };
    static const QUMethod slot_19 = {"rotateCounterClockwise", 0, 0 };
    static const QUMethod slot_20 = {"zoomIn", 0, 0 };
    static const QUMethod slot_21 = {"zoomOut", 0, 0 };
    static const QUMethod slot_22 = {"mirror", 0, 0 };
    static const QUMethod slot_23 = {"moveL", 0, 0 };
    static const QUMethod slot_24 = {"moveR", 0, 0 };
    static const QUMethod slot_25 = {"moveU", 0, 0 };
    static const QUMethod slot_26 = {"moveD", 0, 0 };
    static const QUMethod slot_27 = {"print", 0, 0 };
    static const QUMethod slot_28 = {"toggleDoubleBuffer", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "help()", &slot_0, QMetaData::Public },
	{ "aboutQt()", &slot_1, QMetaData::Private },
	{ "newView()", &slot_2, QMetaData::Private },
	{ "clear()", &slot_3, QMetaData::Private },
	{ "init()", &slot_4, QMetaData::Private },
	{ "addSprite()", &slot_5, QMetaData::Private },
	{ "addCircle()", &slot_6, QMetaData::Private },
	{ "addHexagon()", &slot_7, QMetaData::Private },
	{ "addPolygon()", &slot_8, QMetaData::Private },
	{ "addSpline()", &slot_9, QMetaData::Private },
	{ "addText()", &slot_10, QMetaData::Private },
	{ "addLine()", &slot_11, QMetaData::Private },
	{ "addRectangle()", &slot_12, QMetaData::Private },
	{ "addMesh()", &slot_13, QMetaData::Private },
	{ "addLogo()", &slot_14, QMetaData::Private },
	{ "addButterfly()", &slot_15, QMetaData::Private },
	{ "enlarge()", &slot_16, QMetaData::Private },
	{ "shrink()", &slot_17, QMetaData::Private },
	{ "rotateClockwise()", &slot_18, QMetaData::Private },
	{ "rotateCounterClockwise()", &slot_19, QMetaData::Private },
	{ "zoomIn()", &slot_20, QMetaData::Private },
	{ "zoomOut()", &slot_21, QMetaData::Private },
	{ "mirror()", &slot_22, QMetaData::Private },
	{ "moveL()", &slot_23, QMetaData::Private },
	{ "moveR()", &slot_24, QMetaData::Private },
	{ "moveU()", &slot_25, QMetaData::Private },
	{ "moveD()", &slot_26, QMetaData::Private },
	{ "print()", &slot_27, QMetaData::Private },
	{ "toggleDoubleBuffer()", &slot_28, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Main", parentObject,
	slot_tbl, 29,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Main.setMetaObject( metaObj );
    return metaObj;
}

void* Main::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Main" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool Main::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: help(); break;
    case 1: aboutQt(); break;
    case 2: newView(); break;
    case 3: clear(); break;
    case 4: init(); break;
    case 5: addSprite(); break;
    case 6: addCircle(); break;
    case 7: addHexagon(); break;
    case 8: addPolygon(); break;
    case 9: addSpline(); break;
    case 10: addText(); break;
    case 11: addLine(); break;
    case 12: addRectangle(); break;
    case 13: addMesh(); break;
    case 14: addLogo(); break;
    case 15: addButterfly(); break;
    case 16: enlarge(); break;
    case 17: shrink(); break;
    case 18: rotateClockwise(); break;
    case 19: rotateCounterClockwise(); break;
    case 20: zoomIn(); break;
    case 21: zoomOut(); break;
    case 22: mirror(); break;
    case 23: moveL(); break;
    case 24: moveR(); break;
    case 25: moveU(); break;
    case 26: moveD(); break;
    case 27: print(); break;
    case 28: toggleDoubleBuffer(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Main::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Main::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool Main::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
