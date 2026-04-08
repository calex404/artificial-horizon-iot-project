# Artificial Horizon (IoT Project) ✈️

This project was created for the Digitalization in Industry course at the Faculty of Mechanical Engineering Brno University of Technology (FME BUT).This course is part of the summer semester for 3rd-year students in the Applied Computer Science and Control program. The project was created in collaboration with my colleague who unfortunatelly does not use GitHub account.

## Overview
The goal of this project was to recreate a flight instrument known as an artificial horizon that informs about aircraft's current position to Earth's horizon. The orientation of the embedded system is controlled manually by physical movement. The real-time data is then transmitted and visualized in a web browser, mimicking the behavior of a cockpit's artificial horizon.

## Toolbox
**Hardware:** Wio Terminal
**Communication:** Wi-Fi, MQTT Protocol
**Frontend:** HTML, JavaScript
**Firmware:** C++ (Arduino IDE)

## Contents
`sketch_mar25a.ino` – reads accelerometer data and sends it via MQTT
`index.html` – calculates and visualises real-time data
`*.pdf` – project presentation for the class

## Run project
1. Plug Wio Terminal in the computer (program starts automatically).
2. Launch `index.html` via local server.
3. Go to localhost in your browser.

---

**Name:** Val (calex404), Antonín Vaňha  
**Date:** March 2026
