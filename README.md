# About Draw8!
Draw8! is a multimedia library for Windows platforms (WinRT and Windows Phone 8) aiming to help devs on handling audio, graphics and input.
The engine is composed of different parts that can be loaded depending on the developper will.
For instance, you can use only Draw8.Audio, or just Draw8.Audio and Draw8.Graphics.2D, etc..

Supported platforms :
* Windows Store apps for Windows 8.1 / RT 8.1
* Windows Store apps for Windows 8 / RT 8
* Windows Phone 8

# Modules

## Draw8.Audio
Draw8.Audio is the audio tools module of Draw8!. This part of the engine allows the user to load, generate, and treat sounds simply. Draw8.Audio is using XAudio2.
* Generate a sound manually
* Generate usual sounds (sine wave, square wave, ...)
* Load WAV files
* Apply effects on sounds
* Instanciate sounds to allow them to overlap (ex : gun sounds, ...)
* Change sound frequency at runtime
* Capture audio with the microphone
* Apply filters (low-pass, ...)

+Engine part available+

## Draw8.Graphics.2D
Draw8.Graphics.2D is the part of Draw8! that help the user to display 2D graphics, treat them with filters, load awesome images and draw complex shapes.
+Coming soon+

## Draw8.Graphics.3D
Draw8.Graphics.3D allows the user to display 3D graphics, use awesome shaders on them, procedurally generate meshes, and create great environnements.
+Coming soon+

## Draw8.Input
Draw8.Input helps the user to handle input depending on the device, from touch to keyboard.
+Coming soon+

# Documentation

The complete documentation can be found at http://doc.ilambda.me/draw8/.

# Tutorials

* [Draw8.Audio](#Audio)
* [Draw8.Graphics.2D](#Graphics2D)
* [Draw8.Graphics.3D](#Graphics3D)
* [Draw8.Input](#Input)


## <a name="Audio"></a>Draw8.Audio

### Creating the sound engine
First, to use Draw8.Audio, you must create a *SoundEngine*. This class represents the root all your audio. You can have multiple *SoundEngine* instances at the same time, but an object created using one *SoundEngine* cannot be used by objects of another engine.

To create a sound engine, just do the following :

```csharp
SoundEngine Engine = new SoundEngine();
```

To avoid wasting battery, you should suspend the engine when the app is deactivated, and (re)start it when the app is activated. To achieve this, you can do :
* On Windows phone : 
```csharp
PhoneApplicationService.Current.Deactivated += (s, a) => Engine.Suspend();
PhoneApplicationService.Current.Activated += (s, a) => Engine.Start();
```

* On Windows 8 :
```csharp
Application.Current.Suspending += (s, a) => Engine.Suspend();
Application.Current.Resuming += (s, a) => Engine.Start();
```


### Loading a simple sound
To create a sound, you first need to create an *ISoundSource* with all the informations related to your sound. You will then be able to create multiple instances of this sound to play it multiple times.

There is two ways of creating a *ISoundSource* :
* By loading a file
```csharp
SoundSource Source = new SoundSource(Engine, "MyFile.wav");
```
* By generating the sound data yourself (Hopefully for everyone, SoundData contains static methods which can generate sounds data objects, like sine waves, square waves or white noise)
```csharp
var data = new float[44100];
for(int i = 0 ; i != data.Length ; i++)
    data = (float)Math.Sin(2 * Math.PI * i * (440 / 44100));
SoundSource Source = new SoundSource(Engine, new SoundData(data), Channels.Mono, 44100);
```

Once the source is loaded, you must instantiate it to create a *Sound* (derived from *ISound*) to be able to actually hear the sound. The sound source is what creates the sound, but not the sound itself.

```csharp
Sound Sound = Source.Instantiate(null, null); 
Sound.Play();
```

The first parameter is a *LoopInfo* object : by setting it, you can precisely control if and how your sound is going to loop through time. Set it to null for default parameters (no loop).
The second parameter is the output list for this sound ; for now, set it to null for a direct output on speakers.
Once you created the instance, call Play(), and finally hear your sound !

The point of the sound instance concept is that if you need to play multiple times the same sound while overlaying itself (like the gunshots sounds in a massive cow-boy gunfight), you just instantiate another sound, and let the engine take care of your garbage ! Just don't store unused references.

### Loading a composite sound
A composite sound is a sound with multiple parts that can be looped or not in order to create a bigger, composed sound.
Composite sounds are useful when you want to use the same group of files to create multiple sounds.

You can create a *CompositeSoundSource* - which can be instantiated to be played - with a lot of different ways. 
First, if you're willing to create a source only composed of files, you can just use :
```csharp
Source = new CompositeSoundSource(Engine, new [] { "File1.wav", "File2.wav" }, null);
```

The third argument is an array of *LoopInfo* : you can specify the parts of your sound that are going to be looped, and how many times. Setting it to null will just play the songs one time without any loop.
All the files must have the same format (PCM, Float, ..), the same type (8-bit, 16-bit, ...), and the same sample rate.

If you want to create some or all sound parts manually, just create an array of *SoundData* containing the data to put into your composite sound. Note that you can reuse the *SoundData* for another source ; the buffers can be shared.

```csharp
Data = new [] { SoundData.LoadFromFile(Filename), SoundData.CreateSilence(100) };
Source = new CompositeSoundSource(Engine, Data, SampleRate, Channels);
```

Now, to play the sound, just instantiate it normally :

```csharp
Source.Instantiate(null).Play();
```

The null parameter specify the outputs of the sound. Set it to a correct array of *SoundOutput* to set outputs, and set it to null to output it directly on the speaker(s).

### Sound events and callbacks
You might be in need of a way to know when is a song ending, or when does a CompositeSound goes from one part to another.
Every *ISound* object has events on which the user can add handlers to.
The PartBegin event is triggered when the current *ISound* changes of part. For a simple *Sound*, it means that the sound is simply beginning. For a CompositeSound, it means that the sound is beginning a part or just beginning. 
The PartEnd event is the same event as PartBegin ; except that instead of being trigerred at the beginning, it is triggered at the end.
The LoopEnd event is triggered at the end of a loop section.

Some method calls are not supported into a handler for the *ISound* events.
For instance, you cannot instantiate a *ISoundSource* from one of those callbacks, because of thread issues.
To do so, you can isolate your call in a Dispatcher.RunAsync/Dispatcher.BeginInvoke : the method call will be made on the UI Thread of your app and will not interfere with the sound engine.

### Change outputs and create a sound graph
Sometimes, you might be in need to group some sounds onto a common output to apply the same effects on the sound.
You can, by doing so, create a complex net of outputs.
To add an output on a sound, do the following :

```csharp
SoundTarget Output = new SoundTarget(Engine, Channels, SampleRate, null);
SoundSource.Instanciate(Output);
```

The null parameter in the *SoundTarget* constructor is an array composed of the outputs of the *SoundTarget*. Null means that the output is the speaker itself.

For instance, to apply the same effect on different sounds, do the following :
```csharp
Output = new SoundTarget(Engine, Channels, SampleRate, null);
Output.Effects.Add(new SoundEffect("Reverb"));

SoundInstance1.Instantiate(new [] { Output }).Play();
SoundInstance2.Instantiate(new [] { Output }).Play();
```

### Using filters
You can apply a filter on any *ISoundEmitter* object : that is, a *ISound* or a *SoundTarget*. Four types of filters are available : low pass, band pass, high pass, and notch.
To apply a filter, just do the following (here, a notch filter):

```csharp
Emitter.Filter = new Filter(SoundFilterType.Notch, QualityFactor, CutoffFrequency);
```

The [url:QFactor|http://en.wikipedia.org/wiki/Q_factor] represents the Q-factor (see link) of the filter, aka controlling the filter damping. This parameter must be superior to 2/3.
The [url:CutoffFrequency|http://en.wikipedia.org/wiki/Cutoff_frequency], in Hertz, will represent the frequency at which the filter will be starting to damp input. This parameter must be inferior to the Emitter's sample rate divided by 6.

To delete the filter, just set the Filter property to null. If and only if the property is null, no filter will be applied.

### Using effects
Any *ISoundEmitter* subclass can own and use effects. Effects are like the shaders of audio : they turn sounds in another sounds. Many effects do exist, you can even implement your own effects. 

To use a built-in effect, use :
```csharp
SoundEffect Effect = new SoundEffect(EffectName);
```

EffectName must be the name of a built-in filter. To check currently supported built-in effects, see the full documentation of the *SoundEffect* constructor.

You can also give a ISoundEffectProcessor to implement your own sounds. Create a class that implements ISoundEffectProcessor, and pass it instead of the ElementName.

To add it on a class that support effects (*ISoundEmitter*), you can just do :

```csharp
Emitter.Effects.Add(Effect);
```

To modify your effect's behavior during runtime, you can use the Parameters property of the class SoundEffect to set properties. 

```csharp
Effect.Parameters[ParamName] = Value;
```

The properties will be set just after the value set. ParamName depends of the effect and the parameter you want to modify. Check documentation for more info.

### Acquire sound with a Microphone
You can acquire sound with a capture device, usually a Microphone, with the *Microphone* class.
First, create a *Microphone* class by calling :

```csharp
Microphone Micro = new Microphone(MaxTime);
```

The MaxTime attribute is the maximum duration of time that the microphone will be able to capture.
On Windows Phone 8, the initialization of the microphone is done just after you created it. On windows 8, you should wait the *Microphone*.IsActivated property to be true, as the microphone can only be initialized asynchronously on windows 8. In order to know when is the property changing, subscribe to the Activated event.

Once the mic is activated, you can record sound by calling :

```csharp
Micro.StartRecording();
//Wait some time
SoundData Data = Micro.StopRecording();
```

The StopRecording method will output a *SoundData* valid object that you can use with a *ISoundSource*. The data type is by default 32-bit IEEE-Float Mono at 22050Hz, but you can override this behavior by setting the Type, Format, SampleRate and Channels parameters accordingly.

You can also change the audio input device used by setting the InputDevice parameter in the constructor.
Pass a *ISoundInputDevice* to the constructor to get sound from a given capture device ( [url:XBox-headset|http://msdn.microsoft.com/en-us/library/windows/desktop/ee417001(v=vs.85).aspx#getting_audio_device_identifiers], other microphones, ...).

### Change the audio endpoint
When you create a *SoundEngine*, a constructor overload allows you to pass a *ISoundOutputDevice*. When set to null, the engine will take the default audio output as the audio [url:endpoint|http://en.wikipedia.org/wiki/Communication_endpoint]. 
You can implement this interface to redirect the sound engine output to this endpoint (an external speaker, a [url:XBox-headset|http://msdn.microsoft.com/en-us/library/windows/desktop/ee417001(v=vs.85).aspx#getting_audio_device_identifiers],...) : just redefine the DeviceID property (identifier of the device to receive the output audio) and the DisplayName property to define your endpoint.

### Releasing engine memory
To release memory, you might want to dispose some objects to free memory.
You can (for now) free *ISoundInstance*, *ISound*, and *SoundData* memory. To do so, you just have to call :

```csharp
Object.Dispose();
Object = null; //You should really do this
```

The "object = null" part is not mandatory but recommanded, to make sure you're not going to use a disposed object.


## <a name="Graphics2D"></a>Draw8.Graphics.2D
_Coming soon_


## <a name="Graphics3D"></a>Draw8.Graphics.3D
_Coming soon_


## <a name="Input"></a>Draw8.Input
_Coming soon_

# Special thanks
* DirectXTK (accessible at [url:https://directxtk.codeplex.com]), for the help it provided in the WAV file importer
* French WP/Win8 developper community, for the numerous tutorials they've provided
