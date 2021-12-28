# Infrared proximity sensor
Simple infrared proximity sensor with dynamic adjustment for ambient lighting.

## Intro
This repo contains the source code, raw data, plots, and report for my final project for the NUS EE1111B module, done in June 2021.

It's a simple proximity sensor that works by emitting pulses of infrared radiation and measuring the reflected intensity. The basic idea of measuring reflected light intensity works in theory, but requires tedious tuning to account for ambient infrared light.

This project expands upon the general concept by applying a simple theoretical model based on the inverse-square law to dynamically adjust for ambient lighting conditions, hence only requiring one set of calibration.

[See the full report.](https://github.com/qitianshi/IR-proximity/raw/main/docs/Report.pdf)

## In this repo
- [`IRProximity`](./IRProximity): Arduino source code
- [`data`](./data): Raw data, and generated data based on the inverse-square law model
- [`docs/Report.pdf`](https://github.com/qitianshi/IR-proximity/raw/main/docs/Report.pdf): Full final report
- [`plots`](./plots): Plots of raw and generated data

## License
© 2021 Qi Tianshi.

Source code is licensed under [GNU GPLv3](./LICENSE).

Project report is licensed under [CC BY-NC-ND 4.0](https://creativecommons.org/licenses/by-nc-nd/4.0/).

<p align="center">📏</p>
