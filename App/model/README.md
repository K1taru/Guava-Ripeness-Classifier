# Trained Model

Place your trained model file (.pth) here.

## Expected File

Copy the trained model from `models/` directory:
```
model/
└── guava_classifier_resnet50_final.pth
```

Or any model file like:
```
Guava_resnet50_E15_VAL95.50.pth
```

## Model Requirements

The model file should contain:
- `model_state_dict`: Model weights
- `class_names`: List of class labels  
- `num_classes`: Number of classes
- `model_arch`: 'resnet50' or 'efficientnet_b3'
- `img_size`: Input image size (default: 224x224)
- `normalize_mean`: Normalization mean values
- `normalize_std`: Normalization std values
