"""
Guava Ripeness Dataset Utilities
Handles loading and preprocessing of the Guava Ripeness dataset for age classification.

Dataset Structure Expected:
dataset/
├── Train/
│   ├── day_01/
│   ├── day_02/
│   ├── day_03/
│   └── ...
├── Test/
│   ├── day_01/
│   ├── day_02/
│   └── ...
├── classes.csv (optional - mapping of folder names to labels)
└── README.md

Each class folder contains images representing guavas at that specific age/ripeness stage.
"""

import os
import pandas as pd
import numpy as np
from PIL import Image
from collections import defaultdict
import torch
from torch.utils.data import Dataset


IMAGE_EXTENSIONS = {".jpg", ".jpeg", ".png", ".bmp", ".gif", ".tiff", ".webp"}


class GuavaDataset(Dataset):
    """
    Custom PyTorch Dataset for Guava Ripeness Classification.
    Supports folder-based dataset where each folder represents a ripeness class.
    """
    
    def __init__(self, root_dir, transform=None, class_mapping=None):
        """
        Args:
            root_dir (str): Root directory containing class folders (e.g., Train/ or Test/)
            transform (callable, optional): Optional transform to be applied on images
            class_mapping (dict, optional): Mapping from folder names to class indices
        """
        self.root_dir = root_dir
        self.transform = transform
        self.samples = []  # List of (image_path, class_idx)
        self.classes = []  # List of class names
        self.class_to_idx = {}  # Mapping from class name to index
        
        # Build dataset
        self._build_dataset(class_mapping)
        
    def _build_dataset(self, class_mapping=None):
        """Scan the root directory and build the dataset"""
        if not os.path.exists(self.root_dir):
            raise FileNotFoundError(f"Dataset directory not found: {self.root_dir}")
        
        # Get all class folders (sorted for consistency)
        class_folders = sorted([
            f for f in os.listdir(self.root_dir)
            if os.path.isdir(os.path.join(self.root_dir, f)) and not f.startswith('.')
        ])
        
        if not class_folders:
            raise ValueError(f"No class folders found in {self.root_dir}")
        
        # Use provided mapping or create default (sorted alphabetically/numerically)
        if class_mapping:
            self.class_to_idx = class_mapping
            self.classes = sorted(class_mapping.keys(), key=lambda x: class_mapping[x])
        else:
            self.classes = class_folders
            self.class_to_idx = {cls: idx for idx, cls in enumerate(class_folders)}
        
        # Collect all images
        for class_name in class_folders:
            if class_name not in self.class_to_idx:
                print(f"⚠️ Warning: Folder '{class_name}' not in class mapping, skipping...")
                continue
                
            class_dir = os.path.join(self.root_dir, class_name)
            class_idx = self.class_to_idx[class_name]
            
            for img_name in os.listdir(class_dir):
                ext = os.path.splitext(img_name)[1].lower()
                if ext in IMAGE_EXTENSIONS:
                    img_path = os.path.join(class_dir, img_name)
                    self.samples.append((img_path, class_idx))
        
        print(f"📂 Loaded {len(self.samples)} images from {len(self.classes)} classes")
        
    def __len__(self):
        return len(self.samples)
    
    def __getitem__(self, idx):
        img_path, label = self.samples[idx]
        
        try:
            image = Image.open(img_path).convert('RGB')
        except Exception as e:
            print(f"❌ Error loading image {img_path}: {e}")
            # Return a black image as fallback
            image = Image.new('RGB', (224, 224), color=(0, 0, 0))
        
        if self.transform:
            image = self.transform(image)
            
        return image, label
    
    def get_class_name(self, idx):
        """Get class name from index"""
        for name, i in self.class_to_idx.items():
            if i == idx:
                return name
        return f"Unknown ({idx})"
    
    def get_class_counts(self):
        """Get number of samples per class"""
        counts = defaultdict(int)
        for _, label in self.samples:
            counts[label] += 1
        return dict(counts)


def load_guava_info(dataset_dir):
    """
    Load and analyze Guava dataset information.
    
    Args:
        dataset_dir (str): Path to dataset directory containing Train/ and Test/
        
    Returns:
        dict: Comprehensive dataset statistics
    """
    train_dir = os.path.join(dataset_dir, 'Train')
    test_dir = os.path.join(dataset_dir, 'Test')
    
    # Check directories exist
    train_exists = os.path.exists(train_dir)
    test_exists = os.path.exists(test_dir)
    
    if not train_exists:
        print(f"⚠️ Train directory not found at {train_dir}")
    if not test_exists:
        print(f"⚠️ Test directory not found at {test_dir}")
    
    info = {
        'train_exists': train_exists,
        'test_exists': test_exists,
        'train_dir': train_dir,
        'test_dir': test_dir,
        'num_train': 0,
        'num_test': 0,
        'num_classes': 0,
        'train_class_counts': {},
        'test_class_counts': {},
        'classes': []
    }
    
    if train_exists:
        train_classes = sorted([
            f for f in os.listdir(train_dir)
            if os.path.isdir(os.path.join(train_dir, f)) and not f.startswith('.')
        ])
        info['classes'] = train_classes
        info['num_classes'] = len(train_classes)
        
        for cls in train_classes:
            cls_dir = os.path.join(train_dir, cls)
            count = sum(1 for f in os.listdir(cls_dir) 
                       if os.path.splitext(f)[1].lower() in IMAGE_EXTENSIONS)
            info['train_class_counts'][cls] = count
            info['num_train'] += count
    
    if test_exists:
        test_classes = sorted([
            f for f in os.listdir(test_dir)
            if os.path.isdir(os.path.join(test_dir, f)) and not f.startswith('.')
        ])
        
        for cls in test_classes:
            cls_dir = os.path.join(test_dir, cls)
            count = sum(1 for f in os.listdir(cls_dir) 
                       if os.path.splitext(f)[1].lower() in IMAGE_EXTENSIONS)
            info['test_class_counts'][cls] = count
            info['num_test'] += count
    
    return info


def print_guava_summary(info):
    """
    Print a formatted summary of Guava dataset information.
    
    Args:
        info (dict): Dataset information from load_guava_info()
    """
    print("\n" + "="*80)
    print("🍈 GUAVA RIPENESS DATASET SUMMARY")
    print("="*80)
    
    print(f"\n📊 OVERALL STATISTICS")
    print(f"{'Total Classes:':<30} {info['num_classes']}")
    print(f"{'Training Samples:':<30} {info['num_train']:,}")
    print(f"{'Test Samples:':<30} {info['num_test']:,}")
    print(f"{'Total Samples:':<30} {info['num_train'] + info['num_test']:,}")
    
    if info['classes']:
        print(f"\n📁 RIPENESS CLASSES (Age/Days):")
        for i, cls in enumerate(info['classes']):
            train_count = info['train_class_counts'].get(cls, 0)
            test_count = info['test_class_counts'].get(cls, 0)
            print(f"  {i:2d}. {cls:<20} Train: {train_count:>5} | Test: {test_count:>5}")
    
    if info['train_class_counts']:
        counts = list(info['train_class_counts'].values())
        if counts:
            max_samples = max(counts)
            min_samples = min(counts)
            avg_samples = sum(counts) / len(counts)
            imbalance_ratio = max_samples / min_samples if min_samples > 0 else 0
            
            print(f"\n⚖️  CLASS BALANCE (Training Set)")
            print(f"{'Most populated class:':<30} {max_samples:,} samples")
            print(f"{'Least populated class:':<30} {min_samples:,} samples")
            print(f"{'Average per class:':<30} {avg_samples:.1f} samples")
            print(f"{'Imbalance ratio:':<30} {imbalance_ratio:.2f}x")
            
            if imbalance_ratio > 3:
                print(f"  ⚠️  High class imbalance detected! Consider using weighted sampling.")
            elif imbalance_ratio > 2:
                print(f"  ⚡ Moderate class imbalance. Weighted sampling recommended.")
            else:
                print(f"  ✅ Classes are reasonably balanced.")
    
    print("\n" + "="*80)


def compute_dataset_statistics(dataset):
    """
    Compute mean and standard deviation for normalization.
    
    Args:
        dataset (GuavaDataset): Dataset to compute statistics from
        
    Returns:
        tuple: (mean, std) for RGB channels
    """
    from torch.utils.data import DataLoader
    from torchvision import transforms
    
    print("🔄 Computing dataset mean and std for normalization...")
    print("   This may take a few minutes...")
    
    # Create a temporary dataset without normalization
    temp_transform = transforms.Compose([
        transforms.Resize((224, 224)),
        transforms.ToTensor()
    ])
    
    temp_dataset = GuavaDataset(
        root_dir=dataset.root_dir,
        transform=temp_transform,
        class_mapping=dataset.class_to_idx
    )
    
    loader = DataLoader(temp_dataset, batch_size=64, shuffle=False, num_workers=0)
    
    mean = torch.zeros(3)
    std = torch.zeros(3)
    total_images = 0
    
    for images, _ in loader:
        batch_samples = images.size(0)
        images = images.view(batch_samples, images.size(1), -1)
        mean += images.mean(2).sum(0)
        std += images.std(2).sum(0)
        total_images += batch_samples
    
    mean /= total_images
    std /= total_images
    
    print(f"✅ Dataset statistics computed:")
    print(f"   Mean: [{mean[0]:.4f}, {mean[1]:.4f}, {mean[2]:.4f}]")
    print(f"   Std:  [{std[0]:.4f}, {std[1]:.4f}, {std[2]:.4f}]")
    
    return mean.tolist(), std.tolist()


def create_class_mapping(classes, age_unit='day'):
    """
    Create a class mapping from folder names to indices.
    
    Args:
        classes (list): List of class folder names (e.g., ['day_01', 'day_02', ...])
        age_unit (str): Unit for age labels ('day', 'stage', 'level')
        
    Returns:
        dict: Mapping from folder names to class indices
    """
    class_to_idx = {}
    for idx, cls in enumerate(sorted(classes)):
        class_to_idx[cls] = idx
    return class_to_idx


def get_class_labels(num_classes, age_unit='day'):
    """
    Generate human-readable class labels.
    
    Args:
        num_classes (int): Number of classes
        age_unit (str): Unit for age labels
        
    Returns:
        list: List of class labels
    """
    if age_unit == 'day':
        return [f"Day {i+1}" for i in range(num_classes)]
    elif age_unit == 'stage':
        stages = ['Unripe', 'Slightly Unripe', 'Nearly Ripe', 'Ripe', 'Overripe']
        if num_classes <= len(stages):
            return stages[:num_classes]
        return [f"Stage {i+1}" for i in range(num_classes)]
    else:
        return [f"Class {i}" for i in range(num_classes)]
