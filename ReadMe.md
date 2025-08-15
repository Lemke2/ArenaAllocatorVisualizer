From Ryan Fleury's lecture: "Enter The Arena: Simplifying Memory Management (2023)"
<br>
https://youtu.be/TZ5a3gCCZYo?t=3206

Making a simple (Homogenous) Arena visualizer with a FreeList that tracks empty slots.
<br>
<br>
An Arena is a linear memory management technique, used to simplify object tracking.
<br>
Basically we group objects together by lifetimes and keep them all on the arena. When the lifetime for those objects ends we clear them all together. Due to the nature of cache locality clearing these objects is basically free compared to the cost of doing a malloc and free for each object when we are done with it. This technique is most commonly used for games (Frame buffers as an Arena - all objects life for 1 frame then they are cleared) but can be (AND SHOULD BE) leveraged in all sorts of projects.
