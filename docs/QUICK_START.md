# Quick Start Guide - Guava Ripeness Classifier

## Prerequisites

- Python 3.8 or higher
- NVIDIA GPU with CUDA support (recommended, but CPU works too)
- 8GB RAM minimum, 16GB recommended
- 4GB GPU VRAM minimum (for training)

## Installation

1. **Clone the repository**
```bash
git clone https://github.com/yourusername/Guava-Ripeness-Classifier.git
cd Guava-Ripeness-Classifier
```

2. **Create virtual environment (recommended)**
```bash
python -m venv venv
# Windows
venv\Scripts\activate
# Linux/Mac
source venv/bin/activate
```

3. **Install dependencies**
```bash
pip install -r requirements.txt
```

## Dataset Setup

Organize your guava images in the following structure:

```
dataset/
├── Train/
│   ├── day_01/           # or stage_unripe, level_1, etc.
│   │   ├── image001.jpg
│   │   ├── image002.jpg
│   │   └── ...
│   ├── day_02/
│   │   ├── image001.jpg
│   │   └── ...
│   ├── day_03/
│   └── ... (more age/ripeness classes)
│
└── Test/
    ├── day_01/
    ├── day_02/
    └── ...
```

### Class Naming Convention

You can use any naming convention for your classes:
- **By days**: `day_01`, `day_02`, `day_03`, ...
- **By stage**: `unripe`, `slightly_ripe`, `ripe`, `overripe`
- **By level**: `level_1`, `level_2`, `level_3`, ...

The model will auto-detect classes from folder names.

## Training the Model

1. **Open Jupyter Notebook**
```bash
jupyter notebook src/guava_ripeness_classifier.ipynb
```

2. **Configure settings** in the Global Configuration cell:
```python
MODEL_ARCH = 'resnet50'  # or 'efficientnet_b3'
BATCH_SIZE = 32          # Reduce if GPU memory is limited
MAX_EPOCHS = 30
```

3. **Run all cells** to train the model

4. **Find trained model** in `models/` directory

## Inference (Classifying New Images)

1. **Copy trained model** to `App/model/` folder

2. **Place images** to classify in `App/input/` folder

3. **Open inference notebook**
```bash
jupyter notebook App/Guava_Classifier.ipynb
```

4. **Run all cells** to classify images

## Project Structure

```
Guava-Ripeness-Classifier/
├── src/
│   ├── guava_ripeness_classifier.ipynb  # Main training notebook
│   └── utils/
│       ├── gpu_utils.py         # GPU detection utilities
│       ├── guava_dataset.py     # Dataset loading utilities
│       └── dataset_counter.py   # Dataset statistics
├── App/
│   ├── Guava_Classifier.ipynb   # Inference notebook
│   ├── input/                   # Place images here for classification
│   └── model/                   # Place trained model here
├── dataset/
│   ├── Train/                   # Training images by class
│   └── Test/                    # Test images by class
├── models/                      # Trained models saved here
├── docs/                        # Documentation
├── hardware/                    # Arduino/ESP32 code (legacy)
├── frontend/                    # Vue.js frontend (to be added)
└── requirements.txt
```

## Troubleshooting

### CUDA/GPU Issues
- Ensure NVIDIA drivers are up to date
- Check PyTorch CUDA compatibility: `torch.cuda.is_available()`
- The model will fall back to CPU if GPU is unavailable

### Memory Issues
- Reduce `BATCH_SIZE` in configuration
- Use `efficientnet_b3` which has fewer parameters than `resnet50`

### Dataset Issues
- Ensure images are in supported formats (.jpg, .png, .bmp)
- Check that class folders contain images, not subfolders
- Verify train/test split folders exist

## Next Steps

1. Train the model with your guava dataset
2. Evaluate performance and adjust hyperparameters
3. Deploy using the Vue.js frontend (coming soon)
4. Optionally integrate with hardware (Arduino/ESP32)
