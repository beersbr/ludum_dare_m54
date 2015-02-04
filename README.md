# ludum_dare_m54

A fun game dev project in c++/opengl/json

*jan 4, 2015*

#### TODO (24)
1. ludum_dare_m54/BGL.cpp:455            make sure the sprite is bound
2. ludum_dare_m54/BGL.cpp:477            no lights yet, that will have to be done in the batching
3. ludum_dare_m54/BGL.cpp:508            move this code out int the animation frame functions
4. ludum_dare_m54/BGL.cpp:600            move this code out int the animation frame functions
5. ludum_dare_m54/BGL.h:9                add the sounds component to this.
6. ludum_dare_m54/BGL.h:10               split definitions and code up for creating the lib
7. ludum_dare_m54/BGL.h:95               make this actually set up the render frame for post effects
8. ludum_dare_m54/BGL.h:118              get the frame manager set up so we can run shaders on the entire frame vs each sprite
9. ludum_dare_m54/BGL.h:188              Need to make sure things are named consistently 
10. ludum_dare_m54/BGL.h:201             for batching if it is needed... NOT finished
11. ludum_dare_m54/BGL.h:206             might get some speed boost if the buffer is changed from data to an 
12. ludum_dare_m54/BGL.h:210             Get these finished after we have sprites on the screen
13. ludum_dare_m54/BGL.h:245             this is for a newer version of BGL in preperation for batching
14. ludum_dare_m54/BGLAudio.cpp:61       Before we use this function we need to make sure none of the channels are playing
15. ludum_dare_m54/Entity.h:49           if the component already exists we need to get rid of the old one
16. ludum_dare_m54/Entity.h:74           see if we can manipulate entities purely with data
17. ludum_dare_m54/Entity.h:80           this is just for testing
18. ludum_dare_m54/Entity.h:202          add a transform component that would hold all the transformations
19. ludum_dare_m54/Entity.h:204          that would mean that the renderer needs to change a big to be
20. ludum_dare_m54/Game.cpp:139          Make sure to test the performance of all the glm::functions. If they are taking too long
21. ludum_dare_m54/Game.cpp:153          Need a fast way to get the world coord rect from a sprite. That way 
22. ludum_dare_m54/platform.cpp:189      this is for when I feel like doing cleanup
23. ludum_dare_m54/Player.cpp:67         this is annoying so I disabled it until I can get a different sound lib in 
24. ludum_dare_m54/Player.cpp:80         this is annoying so I disabled it until I can get a different sound lib in 

#### NOTE (33)
1. ludum_dare_m54/BGL.cpp:463            this is a bit redundant... perhaps the texture will have the tag (noted in BGL.h)
2. ludum_dare_m54/BGL.cpp:478            maybe the lights are static parts of the sprite that can be turned
3. ludum_dare_m54/BGL.cpp:500            Do I reset the transform after every draw?
4. ludum_dare_m54/BGL.h:41               perhaps the texture can hold a tag as this structure is
5. ludum_dare_m54/BGLController.cpp:26   Not sure we will need this as the controller seems to have it all
6. ludum_dare_m54/BGLController.cpp:31   this needs to be here so we can clean up later... though right now we
7. ludum_dare_m54/BGLController.cpp:55   What happens if this event has an incorrect 'which'??
8. ludum_dare_m54/BGLController.cpp:58   Controller Ids start at 0
9. ludum_dare_m54/BGLController.cpp:67   I think I can probably do this with a cleverly constructed array
10. ludum_dare_m54/Entity.h:53           this is where we would fetch some data from a json file
11. ludum_dare_m54/Entity.h:85           this will all be moved. Just here for testing.
12. ludum_dare_m54/Entity.h:86           not actually sure this component type is needed at all
13. ludum_dare_m54/Entity.h:160          we can do things based on events here because entities are tagged
14. ludum_dare_m54/Entity.h:161          this function would be called by a collider comopnent in the physics family
15. ludum_dare_m54/Entity.h:186          could grab stuff out of the entities transform data OR could do something if
16. ludum_dare_m54/Entity.h:199          Because the position of the entity is part of the entity
17. ludum_dare_m54/Entity.h:214          Perhaps the entity should just have a list of behavior 'scripts'
18. ludum_dare_m54/Entity.h:226          maybe the behaviors (when loaded, probably at the game load) will be put into a map
19. ludum_dare_m54/Entity.h:252          Entity should provide a pool to add entities to. For now I'm throwing it on the heap
20. ludum_dare_m54/Entity.h:254          this would pull from the kinematic :: create_player_bullet tag and just give the data to the
21. ludum_dare_m54/Game.cpp:59           this is used for the camera
22. ludum_dare_m54/Game.cpp:105          Make sure the player does not leave the current view space
23. ludum_dare_m54/Game.cpp:126          some of the systems are atomic and can be performed concurrently. The behavior
24. ludum_dare_m54/Game.cpp:128          this is where we update each component type using the static update function
25. ludum_dare_m54/Game.cpp:137          dont need to clear as we are drawing a big sprite in the background
26. ludum_dare_m54/Game.cpp:150          With large maps the game hardly runs without culling
27. ludum_dare_m54/platform.cpp:141      ElapsedTick is in ms
28. ludum_dare_m54/platform.cpp:207      This doesnt need to happen if we are covering the entire screen something
29. ludum_dare_m54/platform.cpp:216      This is blocking with vsync on.
30. ludum_dare_m54/Player.cpp:63         This can be done better if we know the current game render rate. 
31. ludum_dare_m54/Player.cpp:69         this works better than the sdl version. Though we just get a filename. it is going to 
32. ludum_dare_m54/Player.cpp:82         this works better than the sdl version. Though we just get a filename. it is going to 
33. ludum_dare_m54/Resourcer.h:10        Dont need these as they cause circular dependencies.