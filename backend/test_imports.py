"""
Simple test to verify backend imports and basic functionality
"""
import sys
print("Python version:", sys.version)

try:
    import flask
    print("✅ Flask imported successfully")
except Exception as e:
    print(f"❌ Flask import failed: {e}")

try:
    import flask_cors
    print("✅ Flask-CORS imported successfully")
except Exception as e:
    print(f"❌ Flask-CORS import failed: {e}")

try:
    import torch
    print(f"✅ PyTorch imported successfully (version {torch.__version__})")
    print(f"   CUDA available: {torch.cuda.is_available()}")
except Exception as e:
    print(f"❌ PyTorch import failed: {e}")

try:
    import torchvision
    print(f"✅ Torchvision imported successfully (version {torchvision.__version__})")
except Exception as e:
    print(f"❌ Torchvision import failed: {e}")

try:
    from PIL import Image
    print("✅ Pillow (PIL) imported successfully")
except Exception as e:
    print(f"❌ Pillow import failed: {e}")

try:
    import numpy
    print(f"✅ NumPy imported successfully (version {numpy.__version__})")
except Exception as e:
    print(f"❌ NumPy import failed: {e}")

print("\n" + "="*50)
print("All core dependencies are installed!")
print("="*50)
