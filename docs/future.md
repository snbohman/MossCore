So everything interacted with the app through systems through contex is through
commands. Commands are stuff like Query, View that results in Pool and Atlas.
These are currently implemented. But I plan on more. So that if you want to quit
you have a quit struct, if you want to attach system you have an attachSystem
struct etc. All of these will then inherit a Command struct.
