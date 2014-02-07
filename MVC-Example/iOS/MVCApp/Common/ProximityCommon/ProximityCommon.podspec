Pod::Spec.new do |s|
    s.name         = 'ProximityCommon'
    s.version      = '1.0.0'
    s.summary      = 'A library that retrieves the current price of your favorite ice cream.' 
    s.author       = { 'Vincil Bishop' => 'YOUR_EMAIL_HERE' }                            
    s.source       = { :git => 'https://github.com/YOUR_GIT_ADDRESS_HERE', :tag => s.version.to_s }      

    s.source_files = 'ProximityCommon/ProximityCommon.h'

    s.resource  = "ProximityCommon.podspec"
    
    s.dependency  'CocoaLumberjack'

    s.subspec "Configuration" do |sp|
        sp.source_files = 'ProximityCommon/PXConfiguration*.{h,m}'
    end

end
