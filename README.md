# RayCastingUpdated

This program was built using SFML, C++, and my own library: SFAS (an addon to SFML).

I used SFML 3.0.0

This built successfully on windows 11 and M2 macbook.

You can find the windows builds in the Builds folder.



This project is split into two parts, the client, and the server.

The server is not required to use the program, only requird if you want to play together.

Controls for the client:

  Arrow keys to look around and move forward/back.
  
  
  To build a wall:
  
  1. select wanted texture from the texture window
  2. go to the little minimap (bottom right) in the main window 
  3. hold down 'Q' this will be the starting point ofd the wall
  4. move the mouse to where you want the end point of the wall to be
  5. let go of 'Q'
  
  The ends of walls will automaticly snap to eachother if they are close enough, if you don't want this to happen
  hold down "Shift" while creating the wall (you can make only one end not snap by only holding down "Shift" when you press OR realease "Q")
  
  
  To delete a wall:
  
  \tDeleting walls is dont by hovering over the wall in the main veiw (the one that looks 3D) and pressing "D", it DOES NOT happen
  \tin the minimap
  
  
  The slider adjusts your height (kinda looks cool).


  The input feild above the 'Connect' button is where you put the IP of a server, (not required to use program) 
  if you leave if blank it will automaticly fill in with "localhost" when you click "connect", if connection is
  successful the connect button will turn green.
  
  
 
Controls for the server:
  
  Run the exe.
  That's it, the server runs of port 3388
 
