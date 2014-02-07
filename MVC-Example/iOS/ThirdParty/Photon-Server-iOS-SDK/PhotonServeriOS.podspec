Pod::Spec.new do |s|
  s.name         = "PhotonServeriOS"
  s.version      = "3.2.5.1"
  s.summary      = "Photon Server iOS SDK"

  s.description  = <<-DESC
                   Photon iOS SDK.
                   DESC

  s.homepage     = "https://www.exitgames.com/en/OnPremise"
  
  s.author       = { "ExitGames" => "sales@exitgames.com" }
  
  s.source       = { :git => "https://github.com/"}

  s.source_files = 'Common-objc/**/*.h', 'Common-cpp/**/*.h', 'Photon-objc/**/*.h', 'Photon-cpp/**/*.h'

  s.framework    = 'UIKit', 'Foundation', 'CoreGraphics'

  s.vendored_libraries = 'Photon-objc/lib/libPhoton-objc_debug_iphoneos.a', 'Photon-objc/lib/libPhoton-objc_debug_iphonesimulator.a', 
'Common-objc/lib/libCommon-objc_debug_iphoneos.a', 'Common-objc/lib/libCommon-objc_debug_iphonesimulator.a'

  s.header_mappings_dir = './'
  
  s.requires_arc = true
  
end