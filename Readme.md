# SFML for ANDROID

You'll find here the detailled steps to **build SFML** and use it in an **Android Studio project**.


## Build SFML 

### get the basics 

- Get **JDK** [here](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html) and install.
- Get **Android Studio** [here](https://developer.android.com/studio/index.html), Install and Run. The SDK will be placed in `$HOME/Library/Android/sdk/`
- Start and Add more packages (LLDB, NDK, CMake, ...) see <https://developer.android.com/ndk/guides/index.html>
- *note: all this will take a lot of space, be ready for it.*
- AndroidStudio will create `$HOME/StudioProjects`
- get Android **NDK-r12b** [here](https://developer.android.com/ndk/downloads/older_releases.html)(we'll need it for SFML, thanks to [Alia5](https://en.sfml-dev.org/forums/index.php?topic=22445.0) for the tip!
- unpack it in `$HOME/Library/Android` rename the folder `ndk_r12b`
- if you need **CMake** get it [here](https://cmake.org/download/). Then add it to the `PATH` in your `.profile` using *nano* or *vi*. `PATH=$PATH:/Applications/CMake.app/Contents/bin`, finally `source $HOME/.profile`

### Let's proceed to building SFML

- open **Terminal**, `cd $HOME/StudioProjects/`
- get **SFML** `git clone https://github.com/SFML/SFML.git`
- as a result you get: `$HOME/StudioProjects/SFML/...`
- set a *SFML* variable: `SFML=$HOME/StudioProjects/SFML`
- go to the SFML folder `cd $SFML` and create a *build* folder `mkdir build`, then `cd build`

### create the *x86* libs

- create an architecture folder `mkdir x86` (to use the emulator) and go there `cd x86`
- create the makefile using cmake `	cmake -DANDROID_ABI=x86 -DCMAKE_TOOLCHAIN_FILE=$SFML/cmake/toolchains/android.toolchain.cmake -DANDROID_NDK=$HOME/Library/Android/ndk_r12b -DCMAKE_FIND_FRAMEWORK="NEVER" -DANDROID_STL=c++_shared -DCMAKE_BUILD_TYPE=Debug $SFML`
- build SFML libs for architecture **x86** `make`

### create the *armeabiXXXX* libs

repeat what you did for the **x86** libs for **armeabi** and **armeabi-v7a** architectures

	cd $SFML/build
	mkdir armeabi
	cd armeabi	

	cmake -DANDROID_ABI=armeabi -DCMAKE_TOOLCHAIN_FILE=$SFML/cmake/toolchains/android.toolchain.cmake -DANDROID_NDK=$HOME/Library/Android/ndk_r12b -DCMAKE_FIND_FRAMEWORK="NEVER" -DANDROID_STL=c++_shared $SFML
	make

and finally

	cd $SFML/build
	mkdir armeabi-v7a
	cd armeabi-v7a	

	cmake -DANDROID_ABI=armeabi-v7a -DCMAKE_TOOLCHAIN_FILE=$SFML/cmake/toolchains/android.toolchain.cmake -DANDROID_NDK=$HOME/Library/Android/ndk_r12b -DCMAKE_FIND_FRAMEWORK="NEVER" -DANDROID_STL=c++_shared $SFML
	make
	
when this is done, put all the libs created somewhere you can find them 

	mkdir $SFML/lib
	cp $SFML/build/x86/* $SFML/lib/x86
	cp $SFML/build/armeabi/* $SFML/lib/armeabi
	cp $SFML/build/armeabi-v7a/* $SFML/lib/armeabi-v7a

also add the extlibsthat aren't statically linked to the SFML libs (the .so).

	cp $SFML/extlibs/libs-android/x86/*.so $SFML/lib/x86
	cp $SFML/extlibs/libs-android/armeabi/*.so $SFML/lib/armeabi
	cp $SFML/extlibs/libs-android/armeabi-v7a/*.so $SFML/lib/armeabi-v7a

you're ready for the second part... build an App.

## Build your SFML App

If you want to use the **Template** you can skip this. I would advise you to try it the long way first (it's good practice).

So, from scratch: Start **Android Studio** and create a **new project** by **importing a sample**. The one you want is **NDK/NativeActivity**. Name your project and ... wait.
You might be requested to update settings to use a newer version of Gradle: do so.

we'll have to change 3 files to make it work:

- AndroidManifest.xml
- build.gradle (module: app)
- CMakeLists.txt

#### AndroidManifest.xml

you'll find it in `Project/app/manifests/`. Change your **App Bundle name** in `<manifest package="my.app.bundle">` to whatever you'd like it to be.

Now the really important bit is in the `<application>` tag. You want to tell NativeActivity to use the **sfml-activity**. To do so, change `android:value="native-activity"` to `android:value="sfml-activity"`.

Then you want to tell SFMLActivity where to find `int main(int argc, char *argv[])`. to do so add a new `<meta-data>` tag after the previous one like this:

	<!-- Tell NativeActivity the name of our activity.so (sfml-activity)-->
	<meta-data android:name="android.app.lib_name"
	  android:value="sfml-activity" /><!-- Never change this! -->
	<!-- Tell sfml-activity (see SFML_Activity.cpp) the name of our app.so -->
	<meta-data android:name="sfml.app.lib_name"
	  android:value="my-app" /><!-- match it to CMakeLists.txt -->
done.

here's a complete example for your reference:

	<?xml version="1.0" encoding="utf-8"?>
	<!-- BEGIN_INCLUDE(manifest) -->
	<!-- change myappname in Gradle:app too -->
	<manifest xmlns:android="http://schemas.android.com/apk/res/android"
	          package="sfml.example.myappname"
	          android:versionCode="1"
	          android:versionName="1.0">
	
	  <!-- This .apk has no Java code itself, so set hasCode to false. -->
	  <application
	      android:allowBackup="false"
	      android:fullBackupContent="false"
	      android:icon="@mipmap/ic_launcher"
	      android:label="SFML TEST"
	      android:hasCode="false">
	
	    <!-- Our activity is the built-in NativeActivity framework class.
	         This will take care of integrating with our NDK code. -->
	    <activity android:name="android.app.NativeActivity"
	              android:label="@string/app_name"
	              android:configChanges="orientation|keyboardHidden">
	      <!-- Tell NativeActivity the name of our activity.so (sfml-activity)-->
	      <meta-data android:name="android.app.lib_name"
	          android:value="sfml-activity" /><!-- Never change this! -->
	      <!-- Tell sfml-activity (see SFML_Activity.cpp) the name of our app.so -->
	      <meta-data android:name="sfml.app.lib_name"
	          android:value="my-app" /><!-- match it to CMakeLists.txt -->
	
	      <intent-filter>
	        <action android:name="android.intent.action.MAIN" />
	        <category android:name="android.intent.category.LAUNCHER" />
	      </intent-filter>
	    </activity>
	  </application>
	
	</manifest>
	<!-- END_INCLUDE(manifest) --> 

## build.gradle (Module: app)

Before doing anything here, copy the **sfml libs** inside your project. 

or even better, create som symbolic links to the libs in `$SFML`

	cd $YOUR_PROJECT
	mkdir sfml
	ln -s $SFML/lib sfml/lib
	ln -s $SFML/include sfml/include 

Your project folder should look like this:

	StudioProjects/MyProject/
		app/
		build/
		gradle/
		sfml/
			include/	# this is a link
				FLAC/stuff.h
				...
				SFML/
				...
				Vorbis/
			lib/ # this is a link
				armeabi/stuff.so
				armeabi-v7a/
				x86/
				mips/
		

you must now change the **App Bundle name** in `defaultConfig { applicationId = '...' }` to the same value you've defined earlier in the **AndroidManisfest.xml** file.

Next, let's specify the **STL** used and the **cppFlags**

	defaultConfig {
	  ...
	  externalNativeBuild {
        cmake {
            cppFlags "-std=c++11 -fexceptions -frtti"
            arguments '-DANDROID_STL=c++_shared'
        }
	  }
	}
        
Now to tell gradle to copy our libs where they can be found by the App, add this block right after `externalNativeBuild { cmake { path 'src/main/cpp/CMakeLists.txt' } }`
	
	// If you want Gradle to package prebuilt native libraries
	// with your APK (you do!), modify the default source set configuration
	// to include the directory of your prebuilt .so files as follows.
	// note: put the sfml libs in MyProject/sfml/lib/(architecture)/libsfml(...).so
	sourceSets {
	    main {
	        jniLibs.srcDirs = ['../sfml/lib/']
	    }
	} 

done.

## CmakeLists.txt

What you want to do here is to specify where the libs are and what to build depending on the architecture required. So here goes


	cmake_minimum_required(VERSION 3.4.1)
		
	set(sfml_dir ${CMAKE_SOURCE_DIR}/../../../../sfml)
	set(sfml_lib ${sfml_dir}/lib/${ANDROID_ABI})
	set(sfml_include ${sfml_dir}/include)
	
	# path to include.hpp files
	include_directories(
	    ${sfml_include}
	    )
	
shared lib will also be tucked into APK and sent to target (refer to `app/build.gradle`, `jniLibs` section for that purpose). We use 
`${ANDROID_ABI}` hereto determine on the fly which architecture to link to (useful if like me you don't have an android device and want to use the simulator)

Next we define where all our libs are:
	
	#--- ADD LIBS: prebuilt libraries (3rd party)
	add_library(sfml-main STATIC IMPORTED)
	set_target_properties(sfml-main PROPERTIES IMPORTED_LOCATION ${sfml_lib}/libsfml-main.a)
	
	add_library(sfml-activity SHARED IMPORTED)
	set_target_properties(sfml-activity PROPERTIES IMPORTED_LOCATION ${sfml_lib}/libsfml-activity.so)
	
	add_library(sfml-system SHARED IMPORTED)
	set_target_properties(sfml-system PROPERTIES IMPORTED_LOCATION ${sfml_lib}/libsfml-system.so)
	
	add_library(sfml-window SHARED IMPORTED)
	set_target_properties(sfml-window PROPERTIES IMPORTED_LOCATION ${sfml_lib}/libsfml-window.so)
	
	add_library(sfml-graphics SHARED IMPORTED)
	set_target_properties(sfml-graphics PROPERTIES IMPORTED_LOCATION ${sfml_lib}/libsfml-graphics.so)
	
	add_library(sfml-audio SHARED IMPORTED)
	set_target_properties(sfml-audio PROPERTIES IMPORTED_LOCATION ${sfml_lib}/libsfml-audio.so)
	
	add_library(openal SHARED IMPORTED)
	set_target_properties(openal PROPERTIES IMPORTED_LOCATION ${sfml_lib}/libopenal.so)
	

finally we can build our final app lib and link all the libs together!

	# --- ADD LIBS: build main app lib
	# Export ANativeActivity_onCreate(),
	# Refer to: https://github.com/android-ndk/ndk/issues/381.
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
	
	# Creates and names a library, sets it as either STATIC
	# or SHARED, and provides the relative paths to its source code.
	# You can define multiple libraries, and CMake builds it for you.
	# Gradle automatically packages shared libraries with your APK.
	# build native_app_glue as a static lib
	# Provides a relative path to your source file(s).
	# Associated headers in the same location as their source
	# file are automatically included.
	add_library(my-app SHARED 
		main.cpp
		)
	
	# --- TARGET
	target_include_directories(my-app PRIVATE
	                           ${sfml_include}
	                           )
	
	target_link_libraries(my-app # specifies the target lib
	    # link the target libs
	
	    #custom_native_app_glue
	    # sfml-main
	
	    android
	    log
	
	    EGL
	    GLESv1_CM
	
	    openal
	    sfml-audio
	
	    sfml-system
	    sfml-window
	    sfml-graphics
	    sfml-activity
	    
		# neat one, thanks Alia5
	    -Wl,--whole-archive sfml-main -Wl,--no-whole-archive
	    )
	
now you can use SFML in your `main.cpp` file.

sync, build, run, *pray* and enjoy.

Lio 

Visit <http://movo-design.com> to see my game TacWars on OSX,IOS and very soon Android.
