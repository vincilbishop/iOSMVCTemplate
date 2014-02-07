Pod::Spec.new do |s|
  s.name         = 'ProximityModel'
  s.version      = '1.0.0'                                                                  
  s.summary      = 'A library that retrieves the current price of your favorite ice cream.' 
  s.author       = { 'Eloy Durán' => 'eloy.de.enige@gmail.com' }                            
  s.source       = { :git => 'https://github.com/alloy/ice-pop.git', :tag => '0.4.2' }      
  s.source_files = 'ProximityModel/ProximityModel.h' 	
  
  s.resource  = "ProximityModel.podspec"
  s.dependency  'ProximityCommon'
  end