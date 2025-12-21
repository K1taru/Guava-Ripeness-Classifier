# 🍈 Guava Ripeness Classifier - Frontend

A user-friendly Vue 3 web interface for the Guava Ripeness Classifier AI system.

## Features

- **🔍 Image Classification** - Upload guava images and get instant ripeness predictions
- **📊 Training Monitor** - Watch AI model training progress in real-time
- **📖 Help & Glossary** - Learn technical terms in plain English
- **📱 Responsive Design** - Works on desktop, tablet, and mobile

## Quick Start

### 1. Install Dependencies

```bash
cd frontend
npm install
```

### 2. Start Development Server

```bash
npm run dev
```

The frontend will be available at `http://localhost:5173`

### 3. Start the Backend (Required)

In a separate terminal:

```bash
cd backend
pip install -r requirements.txt
python app.py
```

The backend will run at `http://localhost:5000`

## Project Structure

```
frontend/
├── src/
│   ├── App.vue              # Main application component
│   ├── main.js              # Vue app entry point
│   ├── style.css            # Global styles
│   └── components/
│       ├── ImageUploader.vue    # Image upload with drag & drop
│       ├── ResultDisplay.vue    # Prediction results display
│       ├── TrainingMonitor.vue  # Real-time training progress
│       └── GlossaryPanel.vue    # Help & terminology guide
├── public/                  # Static assets
├── index.html              # HTML entry point
├── package.json            # Dependencies
└── vite.config.js          # Vite configuration
```

## For Non-Technical Users

This frontend is designed to be easy to use:

1. **Classify Tab** - Just upload a guava photo and click "Analyze"
2. **Training Monitor** - Watch the AI learn (with explanations of what everything means)
3. **Help & Glossary** - Look up any confusing terms

## For Developers

- Built with Vue 3 Composition API (`<script setup>`)
- Uses Vite for fast development
- Communicates with Flask backend via REST API
- Polls for training status updates every 2 seconds

### API Endpoints Used

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/health` | GET | Check backend connection |
| `/api/predict` | POST | Classify an image |
| `/api/training/status` | GET | Get training progress |
| `/api/model/info` | GET | Get model details |

## Build for Production

```bash
npm run build
```

Output will be in the `dist/` folder.
