/**********************************************************************
*  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
*  Copyright (c) 2013, The Pennsylvania State University.
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

#ifndef CONTAM_FORWARDTRANSLATOR_HPP
#define CONTAM_FORWARDTRANSLATOR_HPP

#include "ContamAPI.hpp"

#include <contam/PrjData.hpp>

#include <utilities/idf/Handle.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/StringStreamLogSink.hpp>

namespace openstudio{
class ProgressBar;
namespace model{
class Model;
}

namespace contam{

class CONTAM_API ForwardTranslator
{
public:
  ForwardTranslator();
  boost::optional<std::string> translateToString(const openstudio::model::Model& model,
    bool translateHVAC=true, std::string leakageDescriptor=std::string("Average"));
  bool translate(const openstudio::model::Model& model,bool translateHVAC=true,
    std::string leakageDescriptor=std::string("Average"),ProgressBar *progressBar=0);
  bool translate(const openstudio::model::Model& model,std::map<std::string,int> afeMap,bool translateHVAC=true,
    ProgressBar *progressBar=0);
  bool translate(const openstudio::model::Model& model, double leakageRate=27.1, bool translateHVAC=true, 
    ProgressBar *progressBar=0);
  boost::optional<std::string> toString();
  bool toPrj(const openstudio::path& path);

  static bool modelToPrj(const openstudio::model::Model& model, const openstudio::path& path,
    bool translateHVAC=true, std::string leakageDescriptor=std::string("Average"), ProgressBar* progressBar=NULL);

  bool valid() const {return m_valid && m_data.valid;}
  std::map <Handle, int> surfaceMap() const {return m_surfaceMap;}
  std::map <Handle, int> zoneMap() const {return m_zoneMap;}

  // We will need more functions like this that modify the CONTAM model
  bool setSteadyWeather(double windSpeed, double windDirection);
  int addAirflowElement(std::string name,double flow,double n=0.65,double deltaP=75.0);

  // Get warning messages generated by the last translation.
  std::vector<LogMessage> warnings() const;

  // Get error messages generated by the last translation.
  std::vector<LogMessage> errors() const;

private:
  int tableLookup(QMap<std::string,int> map, std::string str, const char *name);
  int tableLookup(QMap<Handle,int> map, Handle handle, const char *name);
  int tableLookup(std::map<Handle,int> map, Handle handle, const char *name);
  std::string reverseLookup(QMap<std::string,int> map, int nr, const char *name);
  Handle reverseLookup(QMap<Handle,int> map, int nr, const char *name);

  prj::Data m_data;
  // Maps - will be populated after a call of translateToPrj
  // All map to the CONTAM index (1,2,...,nElement)
  QMap<QString,int> m_afeMap;
  QMap <Handle, int> m_levelMap;      // Building story to level map by handle
  std::map <Handle, int> m_zoneMap;       // Thermal zone to airflow zone map by handle
  //QMap <std::string, int> volumeMap; // Map of AHS volumes - may not be needed
  QMap <std::string, int> m_pathMap;  // AHS paths stored by name
  std::map <Handle, int> m_surfaceMap;    // Surface paths stored by handle
  QMap <Handle, int> m_ahsMap;        // Airloop to AHS map by handle

  bool m_valid;

  ProgressBar* m_progressBar;

  StringStreamLogSink m_logSink;

  REGISTER_LOGGER("openstudio.contam.ForwardTranslator");
};

} // contam
} // openstudio

#endif // CONTAM_FORWARDTRANSLATOR_HPP
