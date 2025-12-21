# 🍈 Guava Ripeness Classifier - Backend API

Flask-based REST API for serving the trained PyTorch model and monitoring training progress.

## ⚠️ Python Version Compatibility

**Recommended Python Versions:** 3.11 or 3.12

**Known Issue:** Python 3.14 is very new and has compatibility issues with NumPy/PyTorch on Windows. If you're using Python 3.14, you may encounter segmentation faults.

### Solution Options:

1. **Use Python 3.11 or 3.12** (Recommended)
2. **Use Jupyter Notebook** - Run inference directly in the Jupyter notebooks instead
3. **Wait for package updates** - NumPy and PyTorch will update for Python 3.14 compatibility

## Installation

```bash
cd backend
pip install -r requirements.txt
```

## Running the Backend

```bash
python app.py
```

The server will start at `http://localhost:5000`

## API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/health` | GET | Check if backend is running |
| `/api/predict` | POST | Classify a guava image |
| `/api/training/status` | GET | Get current training status |
| `/api/model/info` | GET | Get loaded model information |
| `/api/model/reload` | POST | Reload model after training |

## Usage Example

### Health Check
```bash
curl http://localhost:5000/api/health
```

### Predict Image
```bash
curl -X POST -F "image=@guava.jpg" http://localhost:5000/api/predict
```

## Model Loading

The backend automatically searches for `.pth` model files in the `../models/` directory and loads the one with the highest validation accuracy (if available in the filename).

## For Developers

### Dependencies

- **Flask** - Web framework
- **Flask-CORS** - Cross-origin resource sharing
- **PyTorch** - Deep learning framework
- **Torchvision** - Computer vision utilities
- **Pillow** - Image processing
- **NumPy** - Numerical computations

### Code Structure

```python
app.py                  # Main Flask application
├── Model Loading       # Auto-detect and load trained models
├── Image Preprocessing # Transform images for inference
├── API Endpoints       # REST API routes
└── Training Monitor    # Real-time training status
```

## Troubleshooting

### ❌ "Import torch could not be resolved"

**Solution:** Install PyTorch
```bash
pip install torch torchvision
```

### ❌ "Segmentation fault" on startup

**Cause:** Python 3.14 compatibility issue with NumPy/PyTorch

**Solution:** Use Python 3.11 or 3.12
```bash
# Check your Python version
python --version

# If 3.14, consider using pyenv or conda to install 3.12
```

### ❌ "No module named 'flask'"

**Solution:** Install Flask
```bash
pip install flask flask-cors
```

### ❌ "No model found"

**Cause:** No trained model in `models/` directory

**Solution:** 
1. Train a model using the Jupyter notebook: `src/guava_ripeness_classifier.ipynb`
2. The trained model will be saved to `models/`
3. Restart the backend

### ⚠️ "Backend Offline" in frontend

**Causes:**
1. Backend is not running
2. Backend crashed on startup
3. Port 5000 is already in use

**Solutions:**
```bash
# Check if backend is running
curl http://localhost:5000/api/health

# Check if port 5000 is in use
netstat -ano | findstr :5000  # Windows
lsof -i :5000                  # Mac/Linux

# Kill process using port 5000 if needed
```

## Alternative: Use Jupyter Notebooks

If you can't get the backend running due to Python version issues, you can still use the classifier directly in Jupyter:

1. **Training:** `src/guava_ripeness_classifier.ipynb`
2. **Inference:** `App/Guava_Classifier.ipynb`

The notebooks work independently of the backend API.

## Production Deployment

For production use:

1. Use a production WSGI server (Gunicorn, uWSGI)
2. Set `debug=False` in `app.run()`
3. Configure proper CORS origins
4. Add authentication/authorization
5. Use environment variables for configuration

Example with Gunicorn:
```bash
pip install gunicorn
gunicorn -w 4 -b 0.0.0.0:5000 app:app
```
