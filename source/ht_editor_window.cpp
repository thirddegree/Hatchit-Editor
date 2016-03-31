/**
**    Hatchit Editor
**    Copyright(c) 2015 Third-Degree
**
**    GNU General Public License
**    This file may be used under the terms of
**    the GNU General Public License version 3 as published by the Free
**    Software Foundation and appearing in the file LICENSE.GPLv3 included
**    in the packaging of this file. Please review the following information
**    to ensure the GNU General Public License requirements
**    will be met: https://www.gnu.org/licenses/gpl.html
**
**/

#include <ht_editor_window.h>
#include <ht_debug.h>
#include <ht_editor_about.h>

#ifdef HT_SYS_WINDOWS
#include <ht_editor_dxview.h>
#endif

#include <ht_editor_glview.h>

#include <QDockWidget>
#include <QFileDialog>
#include <QApplication>
#include <QStyle>
namespace Hatchit {

    namespace Editor {

        Window::Window(QWidget* parent /* = 0 */)
            : QMainWindow(parent)
        {
            m_menuBar = new MenuBar;
            setMenuBar(m_menuBar);

#ifdef HT_SYS_WINDOWS
            m_view = new DXView;
#else
            m_view = new GLView;
#endif
            QDockWidget* sceneDock = new QDockWidget(tr("Scene View"));
            m_sceneTree = new SceneTree;
            sceneDock->setWidget(m_sceneTree);
            addDockWidget(Qt::LeftDockWidgetArea, sceneDock);

            QDockWidget* projViewDock = new QDockWidget(tr("Project View"));
            m_projViewCont = new ProjectViewContainer;
            projViewDock->setWidget(m_projViewCont);
            projViewDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
            projViewDock->setAllowedAreas(Qt::BottomDockWidgetArea);
            addDockWidget(Qt::BottomDockWidgetArea, projViewDock);

            QDockWidget* resourcePrevDock = new QDockWidget(tr("Resource Preview"));
            m_resourcePreview = new ResourcePreview;
            resourcePrevDock->setWidget(m_resourcePreview);
            addDockWidget(Qt::RightDockWidgetArea, resourcePrevDock);


            QWidget* w = new QWidget;
            QHBoxLayout* layout = new QHBoxLayout;
            layout->addWidget(m_view);
            w->setLayout(layout);
            setCentralWidget(w);

            ConnectMenuSlots();

            connect(m_projViewCont->View(), SIGNAL(ImageFileSelected(const QString&)),
                    m_resourcePreview, SLOT(OnImageResourceSelected(const QString&)));
        }

        void Window::OnFileNew()
        {

        }

        void Window::OnFileOpen()
        {
            QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                    "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if(!dir.isEmpty())
            {
                m_projViewCont->View()->SetViewDirectory(dir);
            }
        }

        void Window::OnFileSave()
        {

        }

        void Window::OnFileExit()
        {
            close();
        }

        void Window::OnHelpAbout()
        {
            AboutDialog dialog(this);

            dialog.exec();
        }

        void Window::ConnectMenuSlots()
        {
            connect(m_menuBar->GetFileMenu()->NewProject(), SIGNAL(triggered()),
                this, SLOT(OnFileNew()));
            connect(m_menuBar->GetFileMenu()->OpenProject(), SIGNAL(triggered()),
                this, SLOT(OnFileOpen()));
            connect(m_menuBar->GetFileMenu()->SaveProject(), SIGNAL(triggered()),
                this, SLOT(OnFileSave()));
            connect(m_menuBar->GetFileMenu()->Exit(), SIGNAL(triggered()),
                this, SLOT(OnFileExit()));

            connect(m_menuBar->GetHelpMenu()->About(), SIGNAL(triggered()),
                this, SLOT(OnHelpAbout()));
        }

        void Window::setProjectPath(QString path)
        {
            m_projPath = path;
            m_projViewCont->View()->SetRootDirectory(m_projPath);
            m_projViewCont->View()->SetViewDirectory(m_projPath);
        }
    }

}
