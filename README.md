This is a personal investigation into making a transit planner, mostly inspired by me wanting a project to learn c++ with and some of the cool urbanism job postings i've seen (shoutout to the transit app.)

A little road map for what's to come:

Ideally (although its yet to be implemented), the app will be fed some gtfs data, construct a graph based of said data and then plot an efficient route for the user between any two stops.  Of course, a solid groundwork would then to provide options such as walking but one step at a time.  The goal is then to give this app a small but dockerized python wrapper so that it's more usable and gtfs can be updated with more ease.

Right Now this project is brand new and im just in the research phase (although I have been toying around with things like implementing the university algorithim classic that is dijkstra in c++).  My step by step plans are as follows

1) implement an algorithim to parse GTFS information and construct a representative graph that is actually traversable

2) implement a very basic dijkstra traversal of that graph
2.1) take into account stations that are close enough to transfer by walking
3) expand on dijkstra so that the algorithim actually returns all prevelant information such as travel times, wait times, etc.

4) add a nice little fastAPI wrapper to the app and dockerize it to make it a little more accessible

5) find a way to dynamically update the GTFS graph and add live updates to our graph

6) make graph even more realistic by taking into account stops that may be cancelled or may not run every day


What I'm not looking to implement yet (both due to inexperience with the technology and the fact that already having a full time job makes working on projects difficult)

- multimodal routing (accounting for traveltimes with bikes, walking, etc).  THIS MAY CHANGE DEPENDING ON WHAT I COME ACROSS IN THE PROJECT, IF THERE IS A SIMPLE ENOUGH TECHNOLOGY FOR ROUTING BIKES/WALKING IT COULD ACTUALLY BE QUITE TRIVIAL.  This may be a small project of its own using OSM and then eventually combining it with what i discover here.