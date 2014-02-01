iOSMVCTemplate
==============

A set of XCode templates for an iOS app promoting MVC separation. 

#### TO DO:
* Create 4 separate XCode templates:
** A model Template
** A view template based on an iOS app
** a controller template
** a generic podspec Template
* Make these templates configure to link together with each other by a user configured "solution" name solicited during project creation.
* Investigate how to create an overarching cocoapod that might allow model/view/controller to all be opened in the same project instance.

#### Common Issues:
* When development podspec referenced from another podspec can't be found, make sure that you list the development podspec first so that cocoapods will look at the path specified, and not the public spec repository.
```` 
Fetching podspec for `Your_Podspec` from `../../Path/to/Your_Podspec`
[!] Unable to find a specification for `Another_Development_Podspec`.
````

#### Thanks to:
* http://cocoapods.org/
* https://github.com/NSBoilerplate/Xcode-Project-Templates/wiki/Creating-Xcode-4.x-Project-Templates
* https://github.com/reidmain/Xcode-5-Project-Templates
* https://github.com/borealkiss/Minimal-Template
* http://www.learn-cocos2d.com/store/xcode4-template-documentation/
* https://developer.apple.com/library/ios/documentation/general/conceptual/devpedia-cocoacore/MVC.html



