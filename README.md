# ludum_dare_m54

A fun game dev project in c++/opengl/json

*feb 11, 2015*

## NOTE (31)
1. shaders/spriteEx.vertex:23            remember that these are columns NOT rows
2. ludum_dare_m54/behaviorBase.h:33      we can do things based on events here because entities are tagged
3. ludum_dare_m54/behaviorBase.h:34      this function would be called by a collider comopnent in the physics family
4. ludum_dare_m54/behaviors.h:162        Make sure the player does not leave the current view space
5. ludum_dare_m54/BGL.cpp:469            this is a bit redundant... perhaps the texture will have the tag (noted in BGL.h)
6. ludum_dare_m54/BGL.cpp:484            maybe the lights are static parts of the sprite that can be turned
7. ludum_dare_m54/BGL.cpp:506            Do I reset the transform after every draw?
8. ludum_dare_m54/BGL.h:41               perhaps the texture can hold a tag as this structure is
9. ludum_dare_m54/BGL.h:476              this can probably be done with a single memcpy instead of a loop
10. ludum_dare_m54/BGLController.cpp:26  Not sure we will need this as the controller seems to have it all
11. ludum_dare_m54/BGLController.cpp:31  this needs to be here so we can clean up later... though right now we
12. ludum_dare_m54/BGLController.cpp:55  What happens if this event has an incorrect 'which'??
13. ludum_dare_m54/BGLController.cpp:58  Controller Ids start at 0
14. ludum_dare_m54/BGLController.cpp:67  I think I can probably do this with a cleverly constructed array
15. ludum_dare_m54/components.cpp:34     this is done in the most brute forcey way possible for now. just for testing.
16. ludum_dare_m54/components.h:72       Perhaps the entity should just have a list of behavior 'scripts'
17. ludum_dare_m54/components.h:84       maybe the behaviors (when loaded, probably at the game load) will be put into a map
18. ludum_dare_m54/Entity.cpp:44         ran out of entities
19. ludum_dare_m54/Entity.h:50           this is where we would fetch some data from a json file
20. ludum_dare_m54/EntityFactory.cpp:44  I removed this for now
21. ludum_dare_m54/EntityFactory.cpp:48  the entity no longer holds a reference to a sprite but a sprite component
22. ludum_dare_m54/Game.h:46             Entity should provide a pool to add entities to. For now I'm throwing it on the heap
23. ludum_dare_m54/Game.h:48             this would pull from the kinematic :: create_player_bullet tag and just give the data to the
24. ludum_dare_m54/platform.cpp:140      ElapsedTick is in ms
25. ludum_dare_m54/platform.cpp:206      This doesnt need to happen if we are covering the entire screen something
26. ludum_dare_m54/platform.cpp:215      This is blocking with vsync on.
27. ludum_dare_m54/Player.cpp:26         this is getting removed next commit
28. ludum_dare_m54/Player.cpp:63         This can be done better if we know the current game render rate. 
29. ludum_dare_m54/Player.cpp:69         this works better than the sdl version. Though we just get a filename. it is going to 
30. ludum_dare_m54/Player.cpp:81         this works better than the sdl version. Though we just get a filename. it is going to 
31. ludum_dare_m54/Resourcer.h:10        Dont need these as they cause circular dependencies.

## TODO (24)
1. ludum_dare_m54/behaviors.h:140        should probably move this to the sprite component
2. ludum_dare_m54/BGL.cpp:461            make sure the sprite is bound
3. ludum_dare_m54/BGL.cpp:483            no lights yet, that will have to be done in the batching
4. ludum_dare_m54/BGL.cpp:514            move this code out int the animation frame functions
5. ludum_dare_m54/BGL.cpp:614            move this code out int the animation frame functions
6. ludum_dare_m54/BGL.h:9                add the sounds component to this.
7. ludum_dare_m54/BGL.h:10               split definitions and code up for creating the lib
8. ludum_dare_m54/BGL.h:95               make this actually set up the render frame for post effects
9. ludum_dare_m54/BGL.h:118              get the frame manager set up so we can run shaders on the entire frame vs each sprite
10. ludum_dare_m54/BGL.h:188             Need to make sure things are named consistently 
11. ludum_dare_m54/BGL.h:201             for batching if it is needed... NOT finished
12. ludum_dare_m54/BGL.h:206             might get some speed boost if the buffer is changed from data to an 
13. ludum_dare_m54/BGL.h:210             Get these finished after we have sprites on the screen
14. ludum_dare_m54/BGL.h:246             this is for a newer version of BGL in preperation for batching
15. ludum_dare_m54/BGL.h:686             Get the shader and texture tracking working properly so we dont have to set
16. ludum_dare_m54/BGLAudio.cpp:61       Before we use this function we need to make sure none of the channels are playing
17. ludum_dare_m54/components.cpp:17     I don't wanna do this everytime. Should just make the rect part of
18. ludum_dare_m54/components.cpp:97     sometimes we dont need to draw and rerender things. Especially in the case of the background, we
19. ludum_dare_m54/Entity.cpp:35         do some studying on lists. look for a better way to do this.
20. ludum_dare_m54/Entity.h:46           if the component already exists we need to get rid of the old one
21. ludum_dare_m54/Entity.h:106          see if we can manipulate entities purely with data
22. ludum_dare_m54/Entity.h:116          this is just for testing
23. ludum_dare_m54/platform.cpp:188      this is for when I feel like doing cleanup
24. ludum_dare_m54/Player.cpp:67         this is annoying so I disabled it until I can get a different sound lib in 
