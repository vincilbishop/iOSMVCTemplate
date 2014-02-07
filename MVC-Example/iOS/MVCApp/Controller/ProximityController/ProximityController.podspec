Pod::Spec.new do |s|
  s.name         = 'ProximityController'
  s.version      = '1.0.0'                                                                  
  s.summary      = 'A library that retrieves the current price of your favorite ice cream.' 
  s.author       = { 'Eloy Durán' => 'eloy.de.enige@gmail.com' }                            
  s.source       = { :git => 'https://github.com/alloy/ice-pop.git', :tag => '0.4.2' }      
  
  s.source_files = 'ProximityController/ProximityController.h', 'ProximityController/ProximityControllerConstants.{h,m}'
  
  s.resource  = "ProximityController.podspec"
  
  s.dependency  'ProximityModel'
  s.dependency  'ProximityCommon'
  s.dependency  'CocoaLumberjack'
   
  s.ios.frameworks = 'CFNetwork', 'Security', 'UIKit'
  
  s.ios.xcconfig = {'OTHER_LD_FLAGS' => '-all_load'}

  
  s.subspec "AppController" do |sp|
    sp.source_files = 'ProximityController/RVAppController*.{h,m}'
  end
  
  s.subspec "UserController" do |sp|
    sp.source_files = 'ProximityController/RVUserController*.{h,m}'
  end
end