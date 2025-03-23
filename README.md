# GUI
C++ GUI Library implemented with OpenGL and FreeType

[Button Clicking Demo](https://github.com/YanniSperon/GUI/raw/refs/heads/master/Demo/buttonclicking.mp4)

[Text Wrapping Demo](https://github.com/YanniSperon/GUI/raw/refs/heads/master/Demo/textwrapping.mp4)


## Features

---
- [X] Node Based Layer System (Layers Are Referred To As Views)
  - [X] Modifiable constraints
  - [X] Passing events through views
---
- [X] Empty View
---
- [ ] Button View
  - [X] Event Callbacks
  - [ ] Interact Animation
---
- [X] Image View
---
- [ ] Text View
  - [X] FreeType Font Loading
    - [X] Cached
    - [X] Support for DPI
  - [X] Text Rendering
  - [X] Font Atlas
  - [X] Preloaded Characters For Static Text
  - [ ] Dynamically Loaded Characters For Dynamic Text
  - [ ] Wide Range Of Character Types
    - [X] ASCII
    - [X] Extended ASCII
    - [ ] UTF-8
    - [ ] UTF-16
    - [ ] UTF-32
  - [ ] Text Alignment
    - [X] Horizontal
    - [ ] Vertical
  - [ ] Constraints
    - [X] Number Of Lines
    - [X] Width
    - [ ] Height
  - [ ] Attributed Text Support
    - [X] Vertex Specific Colors
    - [X] Character Specific Font Sizes
    - [X] Character Specific Font Traits (Bold, Italic, etc)
    - [ ] HTML Support
    - [ ] Markdown Support
  - [ ] Text View Itself
    - [ ] Link Text Sizing To View
---
- [ ] Text Input View
  - [ ] Different Types Of Text
    - [ ] Phone Number
    - [ ] Email
    - [ ] Letters
    - [ ] Numbers
    - [ ] Integers
---
- [ ] Table View
  - [ ] Cell View
  - [ ] Controller Delegate
---
- [ ] Scroll View
  - [ ] Horizontal
  - [ ] Vertical
---
- [ ] Support For Custom Gestures (Such As Click & Drag)
  - [X] Single Click
  - [ ] Double Click
  - [ ] Click and Drag
  - [ ] Swipe
---
- [ ] Support For Multiple Rendering APIs
  - [X] OpenGL
  - [ ] Vulkan (Will Eventually Replace OpenGL)
  - [ ] DirectX
  - [ ] Metal
---
