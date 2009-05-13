/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "QmitkStdMultiWidgetEditor.h"

#include <cherryUIException.h>

#include <QWidget>

#include <mitkGlobalInteraction.h>

#include <mitkDataStorageEditorInput.h>

const std::string QmitkStdMultiWidgetEditor::EDITOR_ID = "org.mitk.editors.stdmultiwidget";

QmitkStdMultiWidgetEditor::QmitkStdMultiWidgetEditor()
 : m_StdMultiWidget(0)
{

}

QmitkStdMultiWidgetEditor::~QmitkStdMultiWidgetEditor()
{
  if (m_StdMultiWidget != 0)
    delete m_StdMultiWidget;
}

QmitkStdMultiWidget* QmitkStdMultiWidgetEditor::GetStdMultiWidget()
{
  return m_StdMultiWidget;
}

void QmitkStdMultiWidgetEditor::Init(cherry::IEditorSite::Pointer site, cherry::IEditorInput::Pointer input)
{
  if (input.Cast<mitk::DataStorageEditorInput>().IsNull())
     throw cherry::PartInitException("Invalid Input: Must be IFileEditorInput");

  this->SetSite(site);
  this->SetInput(input);
}

void QmitkStdMultiWidgetEditor::CreateQtPartControl(QWidget* parent)
{
  if (m_StdMultiWidget == 0)
  {
    m_StdMultiWidget = new QmitkStdMultiWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->addWidget(m_StdMultiWidget);

    mitk::DataStorage::Pointer ds = this->GetEditorInput().Cast<mitk::DataStorageEditorInput>()
      ->GetDataStorageReference()->GetDataStorage();

    // Tell the multiWidget which (part of) the tree to render
    m_StdMultiWidget->SetDataStorage(ds);

    // Initialize views as transversal, sagittal, coronar to all data objects in DataStorage
    // (from top-left to bottom)
    mitk::Geometry3D::Pointer geo = ds->ComputeBoundingGeometry3D(ds->GetAll());
    mitk::RenderingManager::GetInstance()->InitializeViews(geo);

    // Initialize bottom-right view as 3D view
    m_StdMultiWidget->GetRenderWindow4()->GetRenderer()->SetMapperID(
      mitk::BaseRenderer::Standard3D );

    // Enable standard handler for levelwindow-slider
    m_StdMultiWidget->EnableStandardLevelWindow();

    // Add the displayed views to the tree to see their positions
    // in 2D and 3D
    m_StdMultiWidget->AddDisplayPlaneSubTree();

    m_StdMultiWidget->EnableNavigationControllerEventListening();

    mitk::GlobalInteraction::GetInstance()->AddListener(
        m_StdMultiWidget->GetMoveAndZoomInteractor()
      );
  }

}

void QmitkStdMultiWidgetEditor::SetFocus()
{
  if (m_StdMultiWidget != 0)
    m_StdMultiWidget->setFocus();
}
