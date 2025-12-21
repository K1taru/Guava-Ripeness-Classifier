"""
Guava Ripeness Classifier - Flask Backend API
==============================================
This backend serves the trained model for inference and provides
training status monitoring for the frontend.

Endpoints:
- GET  /api/health         - Check if backend is running
- POST /api/predict        - Classify a guava image
- GET  /api/training/status - Get current training status
- GET  /api/model/info     - Get loaded model information
"""

import os
import sys
import time
import json
from datetime import datetime
from flask import Flask, request, jsonify
from flask_cors import CORS
from PIL import Image
import torch
import torch.nn as nn
from torchvision import transforms, models
import io

# Add parent directory to path for importing utils
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

app = Flask(__name__)
CORS(app)  # Enable CORS for Vue frontend

# =============================================================================
# CONFIGURATION
# =============================================================================

# Paths
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MODEL_DIR = os.path.join(BASE_DIR, 'models')
DEFAULT_MODEL_PATH = None  # Will be auto-detected

# Model settings
IMG_SIZE = 224
NORMALIZE_MEAN = [0.485, 0.456, 0.406]
NORMALIZE_STD = [0.229, 0.224, 0.225]

# Global variables
model = None
model_info = {
    'loaded': False,
    'name': None,
    'architecture': None,
    'num_classes': 0,
    'class_names': [],
    'accuracy': None
}
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

# Training status (can be updated from training script)
training_status = {
    'is_training': False,
    'current_epoch': 0,
    'total_epochs': 0,
    'train_loss': 0,
    'val_loss': 0,
    'train_acc': 0,
    'val_acc': 0,
    'best_val_acc': 0,
    'learning_rate': 0,
    'start_time': None,
    'logs': [],
    'history': {
        'train_loss': [],
        'val_loss': [],
        'train_acc': [],
        'val_acc': []
    }
}

# =============================================================================
# MODEL LOADING
# =============================================================================

def find_best_model():
    """Find the best model file in the models directory"""
    if not os.path.exists(MODEL_DIR):
        return None
    
    model_files = [f for f in os.listdir(MODEL_DIR) 
                   if f.endswith('.pth') and f.startswith('Guava_')]
    
    if not model_files:
        # Try alternative naming
        model_files = [f for f in os.listdir(MODEL_DIR) 
                       if f.endswith('.pth') and 'guava' in f.lower()]
    
    if not model_files:
        return None
    
    # Sort by validation accuracy if in filename
    def extract_acc(filename):
        try:
            if 'VAL' in filename:
                acc_str = filename.split('VAL')[-1].replace('.pth', '')
                return float(acc_str)
        except:
            pass
        return 0
    
    model_files.sort(key=extract_acc, reverse=True)
    return os.path.join(MODEL_DIR, model_files[0])


def load_model(model_path=None):
    """Load the trained model"""
    global model, model_info
    
    if model_path is None:
        model_path = find_best_model()
    
    if model_path is None or not os.path.exists(model_path):
        print(f"❌ No model found at {model_path}")
        model_info['loaded'] = False
        return False
    
    try:
        print(f"📂 Loading model from: {model_path}")
        checkpoint = torch.load(model_path, map_location=device)
        
        # Get model info from checkpoint
        num_classes = checkpoint.get('num_classes', 5)  # Default to 5 if not specified
        class_names = checkpoint.get('class_names', [f'Class_{i}' for i in range(num_classes)])
        model_arch = checkpoint.get('model_arch', 'resnet50')
        
        # Create model architecture
        if model_arch == 'resnet50':
            model = models.resnet50(weights=None)
            in_features = model.fc.in_features
            model.fc = nn.Sequential(
                nn.Dropout(0.4),
                nn.Linear(in_features, num_classes)
            )
        elif model_arch == 'efficientnet_b3':
            model = models.efficientnet_b3(weights=None)
            in_features = model.classifier[1].in_features
            model.classifier = nn.Sequential(
                nn.Dropout(0.4, inplace=True),
                nn.Linear(in_features, num_classes)
            )
        else:
            raise ValueError(f"Unknown architecture: {model_arch}")
        
        # Load weights
        model.load_state_dict(checkpoint['model_state_dict'])
        model = model.to(device)
        model.eval()
        
        # Update model info
        model_info = {
            'loaded': True,
            'name': os.path.basename(model_path),
            'architecture': model_arch,
            'num_classes': num_classes,
            'class_names': class_names,
            'accuracy': checkpoint.get('val_acc', None)
        }
        
        print(f"✅ Model loaded successfully!")
        print(f"   Architecture: {model_arch}")
        print(f"   Classes: {num_classes} - {class_names}")
        return True
        
    except Exception as e:
        print(f"❌ Error loading model: {e}")
        model_info['loaded'] = False
        return False


# =============================================================================
# IMAGE PREPROCESSING
# =============================================================================

def preprocess_image(image_file):
    """Preprocess uploaded image for model inference"""
    transform = transforms.Compose([
        transforms.Resize((IMG_SIZE, IMG_SIZE)),
        transforms.ToTensor(),
        transforms.Normalize(mean=NORMALIZE_MEAN, std=NORMALIZE_STD)
    ])
    
    # Read image
    image = Image.open(image_file).convert('RGB')
    
    # Apply transforms
    image_tensor = transform(image).unsqueeze(0)  # Add batch dimension
    
    return image_tensor.to(device)


# =============================================================================
# API ENDPOINTS
# =============================================================================

@app.route('/api/health', methods=['GET'])
def health_check():
    """Health check endpoint"""
    return jsonify({
        'status': 'healthy',
        'model_loaded': model_info['loaded'],
        'device': str(device),
        'timestamp': datetime.now().isoformat()
    })


@app.route('/api/predict', methods=['POST'])
def predict():
    """Classify a guava image"""
    if not model_info['loaded']:
        return jsonify({
            'error': 'No model loaded. Please train a model first or place a .pth file in the models/ directory.'
        }), 503
    
    if 'image' not in request.files:
        return jsonify({'error': 'No image file provided'}), 400
    
    image_file = request.files['image']
    
    if image_file.filename == '':
        return jsonify({'error': 'No image selected'}), 400
    
    try:
        start_time = time.time()
        
        # Preprocess image
        image_tensor = preprocess_image(image_file)
        
        # Run inference
        with torch.no_grad():
            outputs = model(image_tensor)
            probabilities = torch.softmax(outputs, dim=1)
            confidence, predicted_idx = torch.max(probabilities, 1)
        
        # Get top predictions
        top_k = min(5, model_info['num_classes'])
        top_probs, top_indices = torch.topk(probabilities, top_k)
        
        top_predictions = []
        for i in range(top_k):
            idx = top_indices[0][i].item()
            prob = top_probs[0][i].item()
            class_name = model_info['class_names'][idx] if idx < len(model_info['class_names']) else f'Class_{idx}'
            top_predictions.append({
                'class': class_name,
                'confidence': prob
            })
        
        processing_time = time.time() - start_time
        
        # Build response
        predicted_class = model_info['class_names'][predicted_idx.item()] if predicted_idx.item() < len(model_info['class_names']) else f'Class_{predicted_idx.item()}'
        
        return jsonify({
            'predicted_class': predicted_class,
            'confidence': confidence.item(),
            'top_predictions': top_predictions,
            'processing_time': processing_time,
            'model_name': model_info['name']
        })
        
    except Exception as e:
        return jsonify({'error': f'Prediction failed: {str(e)}'}), 500


@app.route('/api/training/status', methods=['GET'])
def get_training_status():
    """Get current training status"""
    return jsonify(training_status)


@app.route('/api/training/status', methods=['POST'])
def update_training_status():
    """Update training status (called from training script)"""
    global training_status
    
    data = request.get_json()
    if data:
        training_status.update(data)
        
        # Add log entry if message provided
        if 'message' in data:
            log_entry = {
                'timestamp': datetime.now().strftime('%H:%M:%S'),
                'message': data['message'],
                'type': data.get('log_type', 'info')
            }
            training_status['logs'].append(log_entry)
            # Keep only last 100 logs
            training_status['logs'] = training_status['logs'][-100:]
    
    return jsonify({'status': 'updated'})


@app.route('/api/model/info', methods=['GET'])
def get_model_info():
    """Get information about the loaded model"""
    return jsonify(model_info)


@app.route('/api/model/reload', methods=['POST'])
def reload_model():
    """Reload the model (useful after training)"""
    success = load_model()
    if success:
        return jsonify({
            'status': 'success',
            'model_info': model_info
        })
    else:
        return jsonify({
            'status': 'error',
            'message': 'Failed to load model'
        }), 500


# =============================================================================
# MAIN
# =============================================================================

if __name__ == '__main__':
    print("\n" + "="*60)
    print("🍈 Guava Ripeness Classifier - Backend API")
    print("="*60)
    
    # Try to load model on startup
    load_model()
    
    print(f"\n🖥️  Device: {device}")
    print(f"📁 Models directory: {MODEL_DIR}")
    print(f"🌐 Starting server on http://localhost:5000")
    print("="*60 + "\n")
    
    # Run Flask app
    app.run(host='0.0.0.0', port=5000, debug=True)
