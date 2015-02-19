/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef SDD_FORWARDTRANSLATOR_HPP
#define SDD_FORWARDTRANSLATOR_HPP

#include "SDDAPI.hpp"

#include "../utilities/core/Path.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

#include "../model/ModelObject.hpp"

#include <map>

class QDomDocument;
class QDomElement;
class QDomNodeList;

namespace openstudio {

  class ProgressBar;
  class Transformation;

namespace model {
  class Model;
  class ModelObject;
  class Material;
  class ConstructionBase;
  class Building;
  class BuildingStory;
  class ThermalZone;
  class Space;
  class Surface;
  class SubSurface;
  class ShadingSurface;
  class AirLoopHVAC;
  class FanConstantVolume;
  class CoilCoolingDXSingleSpeed;
  class CoilHeatingGas;
  class AirLoopHVACOutdoorAirSystem;
}

namespace sdd {

  class SDD_API ForwardTranslator {
  public:
    
    ForwardTranslator();

    virtual ~ForwardTranslator();

    bool modelToSDD(const openstudio::model::Model& model, const openstudio::path& path, ProgressBar* progressBar=nullptr);

    /** Get warning messages generated by the last translation.  
      * Warnings are issues encountered during translation that are important, but not fatal to producing a valid SDD for compliance analysis*/
    std::vector<LogMessage> warnings() const;

    /** Get error messages generated by the last translation.
      * Errors are issues encountered during translation that are fatal to producing a valid SDD for compliance analysis*/
    std::vector<LogMessage> errors() const;

  private:

    QString escapeName(const std::string& name);

    // listed in translation order
    // Any of these may throw if they encounter something so terrible they cannot continue,
    // and the caller (modelToSDD) should catch the exception and report the log up to that point.

    // Use LOG(Error ... or LOG(Warn ... to produce log messages.
    // If there is an error that prevents the translation from being able to continue then use LOG_AND_THROW.
    // Prefer LOG(Error over LOG_AND_THROW if possible.
    // Use OS_ASSERT to catch logic errors in the translator implementation.  Do not use OS_ASSERT on bad input, use LOG( instead.

    boost::optional<QDomDocument> translateModel(const openstudio::model::Model& model);
    boost::optional<QDomElement> translateMaterial(const openstudio::model::Material& material, QDomDocument& doc);
    boost::optional<QDomElement> translateConstructionBase(const openstudio::model::ConstructionBase& constructionBase, QDomDocument& doc);
    boost::optional<QDomElement> translateDoorConstruction(const openstudio::model::ConstructionBase& constructionBase, QDomDocument& doc);
    boost::optional<QDomElement> translateFenestrationConstruction(const openstudio::model::ConstructionBase& constructionBase, QDomDocument& doc);
    boost::optional<QDomElement> translateBuilding(const openstudio::model::Building& building, QDomDocument& doc);
    boost::optional<QDomElement> translateBuildingStory(const openstudio::model::BuildingStory& buildingStory, QDomDocument& doc);
    boost::optional<QDomElement> translateSpace(const openstudio::model::Space& space, QDomDocument& doc);
    boost::optional<QDomElement> translateSurface(const openstudio::model::Surface& surface, const openstudio::Transformation& transformation, QDomDocument& doc);
    boost::optional<QDomElement> translateSubSurface(const openstudio::model::SubSurface& subSurface, const openstudio::Transformation& transformation, QDomDocument& doc);
    boost::optional<QDomElement> translateShadingSurface(const openstudio::model::ShadingSurface& shadingSurface, const openstudio::Transformation& transformation, QDomDocument& doc);
    boost::optional<QDomElement> translateThermalZone(const openstudio::model::ThermalZone& thermalZone, QDomDocument& doc);
    boost::optional<QDomElement> translateAirLoopHVAC(const openstudio::model::AirLoopHVAC& airLoop, QDomDocument& doc);
    boost::optional<QDomElement> translateFanConstantVolume(const openstudio::model::FanConstantVolume& fan, QDomElement & airSegElement, QDomDocument& doc);
    boost::optional<QDomElement> translateCoilCoolingDXSingleSpeed(const openstudio::model::CoilCoolingDXSingleSpeed& coil, QDomElement & airSegElement, QDomDocument& doc);
    boost::optional<QDomElement> translateCoilHeatingGas(const openstudio::model::CoilHeatingGas& coil, QDomElement & airSegElement, QDomDocument& doc);
    boost::optional<QDomElement> translateAirLoopHVACOutdoorAirSystem(const openstudio::model::AirLoopHVACOutdoorAirSystem& oasys, QDomElement & airSysElement, QDomDocument& doc);

    std::map<openstudio::Handle, QDomElement> m_translatedObjects;

    // Log untranslated objects as an error,
    // unless the type is in the m_ignoreTypes or m_ignoreObjects member.
    void logUntranslatedObjects(const model::Model& model);

    // Use this member to ignore an entire class of ModelObject.
    std::vector<IddObjectType> m_ignoreTypes;

    // Use this member to ignore a specific ModelObject, but not all of that type.
    // For example you might want to ignore FanConstantSpeed inside of terminals, but 
    // not ignore all FanConstantSpeed objects.
    // There will be no warning that the type is ignored.
    std::vector<Handle> m_ignoreObjects;

    StringStreamLogSink m_logSink;

    ProgressBar* m_progressBar;

    bool m_autoHardSize;
    bool m_autoEfficiency;

    REGISTER_LOGGER("openstudio.sdd.ForwardTranslator");
  };

} // sdd
} // openstudio

#endif // SDD_FORWARDTRANSLATOR_HPP
