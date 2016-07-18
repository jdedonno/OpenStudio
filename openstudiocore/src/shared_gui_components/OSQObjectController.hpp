/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
 *  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef SHAREDGUICOMPONENTS_OSQOBJECTCONTROLLER_HPP
#define SHAREDGUICOMPONENTS_OSQOBJECTCONTROLLER_HPP

#include <QObject>
#include <model/nano_signal_slot.hpp> // Signal-Slot replacement

#include <vector>

namespace openstudio{
  

class OSQObjectController : public QObject, public Nano::Observer
{
  Q_OBJECT

  public:

  /** OSQObjectController manages the life cycle of dynamically allocated QObjects when they
   *  are not part of a parent child hierarchy.  This is the case for top level QWidgets
   *  owned by a controller.  
   **/
  OSQObjectController();

  virtual ~OSQObjectController();

  protected:

  /** Registers the QObject so that its lifecycle is managed.
   *  When OSQObjectController is deleted the managed QObjects will safely be removed.
   **/
  void addQObject(QObject * object);

  private slots:

  void onObjectDestroyed(QObject * object = nullptr);

  private:

  std::vector<QObject *> m_objects;
};


} // openstudio

#endif // SHAREDGUICOMPONENTS_OSQOBJECTCONTROLLER_HPP

