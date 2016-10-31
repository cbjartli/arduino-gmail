# arduino-gmail
This is a simple instructional Arduino project made while co-teaching INFO361 / Advanced Topics in Human-Computer Interaction at the University of Bergen. In this project, the students were asked to come up with situations where tangible user interfaces could be beneficial, and then to carry through the design of a prototype of one such TUI. The students were instructed in the use of design and prototyping tools, and the Arduino platform. 

This project is meant to serve as an example of a simple embedded device software project. The code has been written as close to pure C as possible, with minimal use of C++-specific functionality. 

The project contains the client end of a beginning gmail-TUI. The hardware comprises a WiFi shield, an RGB-LED and an 8x8 LED matrix. The client is intended to connect to a server on the local network, display the number of unread emails in the user's inbox, and indicate the priority of the most important unread email through the color led. This indicator is derived from gmail's labelling system. The project contains a simple discovery protocol, whereby the client discovers any compatible servers by listening for UDP broadcasts. After discovering a server, it starts listening for data packets containing RGB color codes and the number of unread emails. 
