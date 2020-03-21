#ifndef PERMISSION_CHECK_H
#define PERMISSION_CHECK_H

#include <QtGui>
#include <QtWidgets>
#include "util/systemsettings.h"
#include "interface/windowmanager.h"

static void CheckButtonPermissions(QWidget* parent)
{
    int current_level = WindowManager::instance()->GetCurrentUserLevel();

    QList<QObject*> obj_list = parent->findChildren<QObject*>(QString(), Qt::FindChildrenRecursively);

    foreach (QObject* obj, obj_list)
    {
        if( obj->objectName().contains( "btn_"   ) )
        {
            QVariant v = obj->property("DEST");
            if( v.isValid() == false  )
            {
                ((QPushButton*)obj)->setEnabled(false);
                continue;
            }

            if( v.toString() == "ALL" )
            {
                ((QPushButton*)obj)->setEnabled(true);
                continue;
            }

            int level;
            GetConfig(level, "PERMISSIONS/" + v.toString(), 2);

            if( current_level >= level )
                ((QPushButton*)obj)->setEnabled(true);
            else
                ((QPushButton*)obj)->setEnabled(false);
        }
    }
}

#endif // PERMISSION_CHECK_H
