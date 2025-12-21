# Guava Ripeness Classifier 🍈

A deep learning-based guava ripeness/age classifier using PyTorch with ResNet50 and EfficientNet-B3 architectures. This project classifies guava images by their age (days after harvest) or ripeness stage.

[![Python 3.8+](https://img.shields.io/badge/python-3.8+-blue.svg)](https://www.python.org/downloads/)
[![PyTorch](https://img.shields.io/badge/PyTorch-2.0+-red.svg)](https://pytorch.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## 🎯 Project Overview

This project implements a state-of-the-art image classifier for determining guava ripeness based on visual features. It can classify guavas by age (days after harvest) or ripeness stages (unripe, ripe, overripe).

### Key Features

- **Dual Model Architecture Support**: ResNet50 (25.6M params) and EfficientNet-B3 (12M params)
- **Transfer Learning**: Pre-trained on ImageNet for faster convergence
- **Class Imbalance Handling**: Weighted sampling and weighted loss
- **Comprehensive Augmentation**: Rotation, flip, color jitter, perspective transforms
- **Overfitting Prevention**: Dropout, weight decay, early stopping, gradient clipping
- **Easy Inference**: Simple notebook for classifying new images
- **Frontend Ready**: Structure prepared for Vue.js frontend integration
- **Hardware Support**: Legacy Arduino/ESP32 sensor-based classification (in `hardware/` folder)

## 📊 Use Cases

- **Agricultural Research**: Study guava ripening patterns
- **Quality Control**: Automated sorting in fruit processing
- **Smart Farming**: IoT integration for harvest timing
- **Education**: Deep learning for agriculture applications

## 🚀 Quick Start

### Prerequisites

```bash
# Python 3.8 or higher
# NVIDIA GPU with CUDA support (recommended)
# 8GB RAM minimum
```

### Installation

1. **Clone the repository**
```bash
git clone https://github.com/K1taru/Guava-Ripeness-Classifier.git
cd Guava-Ripeness-Classifier
```

2. **Install dependencies**
```bash
pip install -r requirements.txt
```

3. **Prepare your dataset** (see [Dataset Setup](#dataset-setup))

4. **Train the model**
```bash
jupyter notebook src/guava_ripeness_classifier.ipynb
```

See [`docs/QUICK_START.md`](docs/QUICK_START.md) for detailed instructions.

## 📁 Project Structure

```
Guava-Ripeness-Classifier/
├── src/                              # Source code
│   ├── guava_ripeness_classifier.ipynb   # Main training notebook
│   └── utils/                        # Utility modules
│       ├── gpu_utils.py              # GPU detection utilities
│       ├── guava_dataset.py          # Dataset loading utilities
│       └── dataset_counter.py        # Dataset statistics
│
├── App/                              # Inference application
│   ├── Guava_Classifier.ipynb        # Inference notebook
│   ├── input/                        # Place images here for classification
│   └── model/                        # Place trained model here
│
├── dataset/                          # Dataset folder
│   ├── Train/                        # Training images by class
│   │   ├── day_01/
│   │   ├── day_02/
│   │   └── ...
│   └── Test/                         # Test images by class
│
├── models/                           # Trained models saved here
├── docs/                             # Documentation
├── frontend/                         # Vue.js frontend (to be implemented)
├── hardware/                         # Legacy Arduino/ESP32 code
│   ├── guava_v1.ino
│   ├── guava_v2.ino
│   └── guava_v3.ino
│
├── requirements.txt                  # Python dependencies
├── .gitignore
├── LICENSE
└── README.md
```

## 📂 Dataset Setup

Organize your guava images by class (age/ripeness stage):

```
dataset/
├── Train/
│   ├── day_01/     # Images from Day 1 after harvest
│   ├── day_02/     # Images from Day 2
│   ├── day_03/     # Images from Day 3
│   └── ...
└── Test/
    ├── day_01/
    ├── day_02/
    └── ...
```

**Tips:**
- Minimum 100 images per class recommended
- Use consistent lighting and background
- Include variety in size and orientation

## 🧠 Model Training

The training notebook provides:

1. **GPU Detection**: Automatic CUDA/CPU detection
2. **Dataset Analysis**: Class distribution and statistics
3. **Data Augmentation**: Configurable transforms
4. **Training Loop**: With early stopping and LR scheduling
5. **Evaluation**: Confusion matrix, classification report
6. **Model Export**: Ready for inference

### Configuration Options

```python
MODEL_ARCH = 'resnet50'      # or 'efficientnet_b3'
BATCH_SIZE = 32              # Adjust based on GPU memory
MAX_EPOCHS = 30
LEARNING_RATE = 0.0001
USE_AUGMENTATION = True
USE_WEIGHTED_SAMPLER = True  # For imbalanced datasets
```

## 📱 Inference

1. Copy trained model to `App/model/`
2. Place images in `App/input/`
3. Run `App/Guava_Classifier.ipynb`

## 🔧 Hardware (Legacy)

The `hardware/` folder contains Arduino code for sensor-based ripeness detection using:
- pH sensor
- MQ135 (VOCs/CO2)
- MQ3 (ethylene/aromatics)
- I2C 20x4 LCD display

## 🌐 Frontend (Planned)

A Vue.js frontend is planned for easy web-based classification:
- Drag-and-drop image upload
- Real-time classification
- Mobile-responsive design

## 📈 Performance

*To be updated after training on your dataset*

| Model | Parameters | Accuracy | Inference Time |
|-------|------------|----------|----------------|
| ResNet50 | 25.6M | TBD | ~50ms |
| EfficientNet-B3 | 12M | TBD | ~40ms |

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- PyTorch team for the deep learning framework
- torchvision for pre-trained models
- German Traffic Sign Classifier project for architectural inspiration
