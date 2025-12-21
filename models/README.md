# Trained Models

This folder stores trained model checkpoints.

## Model Files

After training, you'll find models named like:
```
Guava_resnet50_E15_VAL95.50.pth
Guava_efficientnet_b3_E20_VAL97.25.pth
guava_classifier_resnet50_final.pth
```

### Naming Convention
- `Guava_`: Prefix for guava classifier models
- `resnet50` / `efficientnet_b3`: Model architecture
- `E15`: Epoch number
- `VAL95.50`: Validation accuracy percentage
- `_final`: Final exported model for inference

## Model Contents

Each `.pth` file contains:
- `model_state_dict`: Model weights
- `optimizer_state_dict`: Optimizer state (for resuming training)
- `epoch`: Training epoch
- `val_acc`: Validation accuracy
- `val_loss`: Validation loss
- `class_names`: List of class labels
- `num_classes`: Number of classes
- `model_arch`: Architecture name ('resnet50' or 'efficientnet_b3')

## Using Models for Inference

1. Copy the desired model to `App/model/`
2. Open `App/Guava_Classifier.ipynb`
3. The notebook will automatically load the model

## Model Size Reference

- **ResNet50**: ~100 MB
- **EfficientNet-B3**: ~50 MB

## Git LFS

Large model files (>.pth) are tracked with Git LFS.
If you need the actual model files, ensure Git LFS is installed:
```bash
git lfs install
git lfs pull
```
