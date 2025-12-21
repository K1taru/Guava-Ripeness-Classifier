# Guava Ripeness Dataset

This folder should contain your guava images organized by ripeness class (age in days).

## Expected Structure

```
dataset/
├── Train/
│   ├── day_01/
│   │   ├── guava_001.jpg
│   │   ├── guava_002.jpg
│   │   └── ...
│   ├── day_02/
│   │   └── ...
│   ├── day_03/
│   │   └── ...
│   └── ... (more classes)
│
├── Test/
│   ├── day_01/
│   ├── day_02/
│   └── ...
│
└── README.md (this file)
```

## Guidelines for Dataset Creation

### Image Collection
1. **Consistent lighting**: Use consistent lighting conditions
2. **Background**: Use a plain, contrasting background
3. **Multiple angles**: Capture guavas from different angles
4. **Resolution**: Minimum 224x224 pixels recommended

### Class Organization
- Create one folder per ripeness stage/day
- Folder names will be used as class labels
- Recommended: 100+ images per class for good results
- Aim for balanced classes (similar number of images per class)

### Naming Convention Options

**By Days (Age)**:
```
day_01/  → Freshly harvested (Day 1)
day_02/  → Day 2 after harvest
day_03/  → Day 3 after harvest
...
day_07/  → Day 7 (likely overripe)
```

**By Ripeness Stage**:
```
01_unripe/
02_slightly_unripe/
03_nearly_ripe/
04_ripe/
05_overripe/
```

**By Custom Labels**:
```
stage_A/
stage_B/
stage_C/
```

## Train/Test Split

- **Train folder**: ~80% of your images (used for training)
- **Test folder**: ~20% of your images (used for final evaluation)

Make sure both Train and Test folders have the same class subfolders.

## Supported Image Formats

- `.jpg` / `.jpeg`
- `.png`
- `.bmp`
- `.tiff`
- `.webp`

## Tips for Better Results

1. **Data Augmentation**: The training script automatically applies augmentation
2. **More Data**: More images generally lead to better accuracy
3. **Quality over Quantity**: Ensure images are clear and representative
4. **Class Balance**: Try to have similar number of images per class
5. **Real-world Variety**: Include variations in size, shape, and minor defects
