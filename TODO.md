## TODO

### Level Generation 
- Map prefabs to candidate rooms based on where the doors are + which entities are impassable
    - Or maybe just delete entities as necessary to ensure room access?
- Rotate prefabs to match room alignment
- Tweak room-size generation to make slightly more sensible distribution of rooms 
- Specially sized/shaped rooms e.g. rounded rooms 
- Switch to different level layout generation for later levels? 

### FOV 
- Entities marked as non-light blocking shouldn't block light 
- Some entities shouldn't be visible within fog of war (ones which might move!)

### Gameplay 
- Implement (re-implement) a level-controller system to handle the different UI states we might be in  

### UI
- Finish text log 
    -  Extension - including scrollback
- Add dialogs
    - Add message boxes and confirmation boxes
- Add character info view 
- Add description view 
- Add spell/ability/feat interface 
- Add menus of all sorts 