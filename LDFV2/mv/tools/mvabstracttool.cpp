#include "mvabstracttool.h"

/// ===========================================================================
///
/// ===========================================================================
MvAbstractTool::MvAbstractTool(const QRectF& rect, QGraphicsItem *parent) :
    MvTool(rect, parent), pLayout(nullptr)
{
    executionOrder = 0;
    bShowThreshold  = false;

    mv_type = MV_TYPE_UNDEFINED;
}

/// ===========================================================================
///
/// ===========================================================================
MvAbstractTool::~MvAbstractTool()
{

}

/// ===========================================================================
///
/// ===========================================================================
void MvAbstractTool::SetToolName(const QString &name)
{
    MvTool::SetToolName( name );
    emit(NameChanged(name));
}

/// ===========================================================================
///
/// ===========================================================================
void MvAbstractTool::ReconfigPosition()
{
    configPos = mapToScene(this->boundingRect().center());
}

/// ===========================================================================
///
/// ===========================================================================
void MvAbstractTool::SetTableDataBase(QMap<QString, QString> _table)
{
    table = _table;
}

/// ===========================================================================
///
/// ===========================================================================
void MvAbstractTool::SetAttributeDataBase(const QString &value)
{
     attribute=value;
}

/// ===========================================================================
///
/// ===========================================================================
QString MvAbstractTool::GetAttributeDataBase() const
{
    return attribute;
}

/// ===========================================================================
///
/// ===========================================================================
void MvAbstractTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    MvTool::mouseMoveEvent( event );
    configPos = mapToScene(this->boundingRect().center());
}

