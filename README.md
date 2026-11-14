# Foundations of IoT – Experiments Repository

## 1. About

This repository contains the sample structure and submission guidelines for IoT laboratory experiments.

Students must organize their experiment submissions according to the hardware platform used and maintain the required documentation for every experiment.

## 2. Repository Structure

The repository must follow the directory structure shown below:

```text
Foundations_of_IoT/
│
├── README.md
├── .gitignore
│
├── 8051/
│   ├── CAT1/
│   │   ├── README.md
│   │   ├── circuit.png
│   │   └── project files
│   │
│   └── CAT2/
│       ├── README.md
│       ├── circuit.png
│       └── project files
│
└── STM32/
    ├── CAT1/
    │   ├── README.md
    │   ├── circuit.png
    │   └── project files
    │
    └── CAT2/
        ├── README.md
        ├── circuit.png
        └── project files
```

**Note:** The above structure is illustrative. The number of experiments and their identifiers will depend on the experiments performed.

## 3. Submission Guidelines

### 3.1 Folder Organization

- Create a separate folder for each hardware platform.
- Create a separate subfolder for every experiment under the respective platform folder.
- Name experiment folders using the prescribed experiment identifier (e.g., CAT1, CAT2, CAT3).
- Do not combine multiple experiments into a single experiment folder.
- Maintain the same directory structure throughout the repository.

### 3.2 Required Documentation

Every experiment folder must contain a `README.md` file with the following sections:

| Field | Description |
|---|---|
| EXP | Experiment identifier |
| DATE | Date of experiment |
| NAME OF EXP | Title of the experiment - follow what is given in the questions pdf |
| HARDWARE SETUP / PLATFORM | Board, microcontroller and peripherals used |
| TASK NO | Experiment task - follow what is given in the questions pdf |
| Circuit Diagram | Hardware connection diagram |
| Core Logic / Pseudocode / Code Base | Explanation of the implementation |
| INFERENCE | Observation and conclusion |

### 3.3 Circuit Diagram Requirements

The circuit diagram must clearly indicate:

- Development board used.
- Microcontroller IC and its model.
- GPIO pins and other pins used.
- External peripherals and components.
- Connections between the components.
- Power supply and ground connections wherever applicable.

The diagram must accurately represent the hardware setup used for the experiment.

Students can use circuit simulation tools such as Wokwi or Tinkercad, where supported, or suitable schematic-drawing tools or can also manually draw it in paper and upload.

Circuit diagrams must be saved inside the respective experiment folder and referenced in the experiment's `README.md`.

### 3.4 Source Code and Project Files

Each experiment folder must contain the relevant source code and project configuration files required to open, understand and reproduce the experiment.

Examples include:

**8051:**
- Embedded C source files (`.c`).
- Keil project files (`.uvproj`).
- Required startup files (`STARTUP.A51`).
- Compiled firmware (`.hex`), where applicable.

**STM32:**
- STM32CubeMX configuration file (`.ioc`).
- Application source files (`Core/`).
- Required HAL drivers (`Drivers/`).
- Keil project files (`.uvprojx`), where applicable.
- Compiled firmware (`.hex`), where applicable.

Do not include unnecessary IDE user settings, temporary files or generated intermediate build files.

## 4. README Format

Every experiment must follow the prescribed documentation format.

The README must contain all required sections, with relevant information filled in for the experiment.

Circuit diagrams must be embedded using relative file paths.

Example:

```markdown
![Circuit Diagram](circuit.png)
```

## 5. General Instructions

1. Maintain the prescribed folder structure.
2. Ensure that every experiment has its own README.md.
3. Use meaningful and consistent file and folder names.
4. Include all necessary project and source files.
5. Ensure circuit diagrams are clear and correctly labelled.
6. Verify that all required files are present before submitting the repository.
7. Keep the GitHub repository accessible to the evaluator.

## 6. Sample Experiments

This repository contains sample experiments demonstrating the expected folder organization, documentation format and project file structure.

Students should use these samples as a reference when preparing their own experiment submissions.
