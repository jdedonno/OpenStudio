########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class ISOModel_Test < MiniTest::Unit::TestCase

  def test_ISOModel
  
    model = OpenStudio::Model::exampleModel
    epw_path = OpenStudio::Path.new($OpenStudio_ResourcePath + "runmanager/USA_CO_Golden-NREL.724666_TMY3.epw") 
    
    ft = OpenStudio::ISOModel::ISOModelForwardTranslator.new
    userModel = ft.translateModel(model)
    userModel.setWeatherFilePath(epw_path)
    simModel = userModel.toSimModel()
    results = simModel.simulate()

    monthlyResults = results.monthlyResults
    assert_equal(12, monthlyResults.size)
    
    fuelTypes = OpenStudio::EndUses.fuelTypes
    puts "Month, #{fuelTypes.join(',')} kBtu/ft^2"
    
    month = 1
    monthlyResults.each do |monthlyResult|
      endUses = []
      fuelTypes.each do |fuelType| 
        endUse = OpenStudio::convert(monthlyResult.getEndUseByFuelType(fuelType), 'kWh/m^2', 'kBtu/ft^2').get
        endUses << endUse
      end
      puts "#{OpenStudio::MonthOfYear.new(month).valueName}, #{endUses.join(',')}"
      month += 1
    end
    eui = OpenStudio::convert(results.totalEnergyUse, 'kWh/m^2', 'kBtu/ft^2').get
    puts "Total energy use intensity: #{eui} kBtu/ft^2"
    assert(eui > 0)
    
    floorArea = OpenStudio::convert(userModel.floorArea, 'm^2', 'ft^2').get
    puts "Floor area: #{floorArea} ft^2"
  end
  
end
