<script setup>
import { ref, onMounted, onUnmounted } from 'vue'
import ImageUploader from './components/ImageUploader.vue'
import ResultDisplay from './components/ResultDisplay.vue'
import TrainingMonitor from './components/TrainingMonitor.vue'
import GlossaryPanel from './components/GlossaryPanel.vue'

// Active tab
const activeTab = ref('classify')

// Classification state
const prediction = ref(null)
const loading = ref(false)
const error = ref(null)

// Training state
const trainingStatus = ref({
  is_training: false,
  current_epoch: 0,
  total_epochs: 0,
  train_loss: 0,
  val_loss: 0,
  train_acc: 0,
  val_acc: 0,
  best_val_acc: 0,
  learning_rate: 0,
  logs: [],
  history: {
    train_loss: [],
    val_loss: [],
    train_acc: [],
    val_acc: []
  }
})

// Backend connection status
const backendConnected = ref(false)
const backendUrl = ref('http://localhost:5000')

// Polling interval
let statusInterval = null

// Check backend connection
const checkBackendConnection = async () => {
  try {
    const response = await fetch(`${backendUrl.value}/api/health`, {
      method: 'GET',
      signal: AbortSignal.timeout(3000)
    })
    backendConnected.value = response.ok
  } catch {
    backendConnected.value = false
  }
}

// Fetch training status
const fetchTrainingStatus = async () => {
  if (!backendConnected.value) return
  
  try {
    const response = await fetch(`${backendUrl.value}/api/training/status`)
    if (response.ok) {
      const data = await response.json()
      trainingStatus.value = { ...trainingStatus.value, ...data }
    }
  } catch (err) {
    console.error('Failed to fetch training status:', err)
  }
}

// Handle classification
const handlePrediction = async (imageFile) => {
  loading.value = true
  error.value = null
  prediction.value = null

  try {
    const formData = new FormData()
    formData.append('image', imageFile)

    const response = await fetch(`${backendUrl.value}/api/predict`, {
      method: 'POST',
      body: formData,
    })

    if (!response.ok) {
      throw new Error(`Server error: ${response.status}`)
    }

    const data = await response.json()
    prediction.value = data
  } catch (err) {
    error.value = err.message || 'Failed to get prediction. Make sure the backend server is running.'
    console.error('Prediction error:', err)
  } finally {
    loading.value = false
  }
}

const handleReset = () => {
  prediction.value = null
  error.value = null
}

// Lifecycle
onMounted(() => {
  checkBackendConnection()
  // Poll for status every 2 seconds
  statusInterval = setInterval(() => {
    checkBackendConnection()
    if (activeTab.value === 'training') {
      fetchTrainingStatus()
    }
  }, 2000)
})

onUnmounted(() => {
  if (statusInterval) {
    clearInterval(statusInterval)
  }
})
</script>

<template>
  <div id="app">
    <!-- Header -->
    <header>
      <div class="header-content">
        <h1>🍈 Guava Ripeness Classifier</h1>
        <p class="tagline">AI-powered guava age detection using deep learning</p>
        
        <!-- Connection Status -->
        <div class="connection-status" :class="{ connected: backendConnected }">
          <span class="status-dot"></span>
          <span v-if="backendConnected">Backend Connected</span>
          <span v-else>Backend Offline</span>
        </div>
      </div>
    </header>

    <!-- Navigation Tabs -->
    <nav class="tab-navigation">
      <button 
        :class="{ active: activeTab === 'classify' }"
        @click="activeTab = 'classify'"
      >
        <span class="tab-icon">🔍</span>
        <span class="tab-label">Classify Image</span>
        <span class="tab-desc">Upload & analyze guava</span>
      </button>
      <button 
        :class="{ active: activeTab === 'training' }"
        @click="activeTab = 'training'; fetchTrainingStatus()"
      >
        <span class="tab-icon">📊</span>
        <span class="tab-label">Training Monitor</span>
        <span class="tab-desc">View model training</span>
      </button>
      <button 
        :class="{ active: activeTab === 'help' }"
        @click="activeTab = 'help'"
      >
        <span class="tab-icon">📖</span>
        <span class="tab-label">Help & Glossary</span>
        <span class="tab-desc">Learn the terms</span>
      </button>
    </nav>

    <!-- Main Content -->
    <main>
      <!-- Classify Tab -->
      <div v-show="activeTab === 'classify'" class="tab-content">
        <div class="intro-card">
          <h2>🔬 How It Works</h2>
          <div class="steps">
            <div class="step">
              <span class="step-num">1</span>
              <span class="step-text">Upload a photo of your guava</span>
            </div>
            <div class="step">
              <span class="step-num">2</span>
              <span class="step-text">Our AI analyzes the image</span>
            </div>
            <div class="step">
              <span class="step-num">3</span>
              <span class="step-text">Get the ripeness stage prediction</span>
            </div>
          </div>
        </div>

        <div class="container">
          <ImageUploader 
            @predict="handlePrediction"
            @reset="handleReset"
            :loading="loading"
            :disabled="!backendConnected"
          />

          <div v-if="!backendConnected" class="warning-message">
            <p>⚠️ Backend server is not running</p>
            <p class="warning-hint">
              Start the Python backend server first:<br>
              <code>cd backend && python app.py</code>
            </p>
          </div>

          <div v-if="error" class="error-message">
            <p>❌ {{ error }}</p>
            <p class="error-hint">
              💡 Make sure the Python backend is running on port 5000
            </p>
          </div>

          <ResultDisplay 
            v-if="prediction"
            :prediction="prediction"
          />
        </div>
      </div>

      <!-- Training Monitor Tab -->
      <div v-show="activeTab === 'training'" class="tab-content">
        <TrainingMonitor 
          :status="trainingStatus"
          :connected="backendConnected"
          :backend-url="backendUrl"
          @refresh="fetchTrainingStatus"
        />
      </div>

      <!-- Help & Glossary Tab -->
      <div v-show="activeTab === 'help'" class="tab-content">
        <GlossaryPanel />
      </div>
    </main>

    <!-- Footer -->
    <footer>
      <p>Powered by PyTorch & Vue.js | Guava Ripeness Detection System</p>
      <p class="version">v1.0.0 | For Research & Educational Purposes</p>
    </footer>
  </div>
</template>

<style scoped>
#app {
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  background: #f7fafc;
}

/* Header */
header {
  background: linear-gradient(135deg, #38a169 0%, #276749 100%);
  color: white;
  padding: 2rem 1rem;
  text-align: center;
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.15);
}

.header-content h1 {
  margin: 0 0 0.5rem 0;
  font-size: 2.5rem;
  font-weight: 700;
}

.tagline {
  margin: 0 0 1rem 0;
  opacity: 0.9;
  font-size: 1.1rem;
}

.connection-status {
  display: inline-flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.5rem 1rem;
  background: rgba(255, 255, 255, 0.15);
  border-radius: 20px;
  font-size: 0.85rem;
}

.status-dot {
  width: 10px;
  height: 10px;
  border-radius: 50%;
  background: #fc8181;
  animation: pulse 2s infinite;
}

.connection-status.connected .status-dot {
  background: #68d391;
}

@keyframes pulse {
  0%, 100% { opacity: 1; }
  50% { opacity: 0.5; }
}

/* Tab Navigation */
.tab-navigation {
  display: flex;
  justify-content: center;
  gap: 1rem;
  padding: 1rem;
  background: white;
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.05);
  flex-wrap: wrap;
}

.tab-navigation button {
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 1rem 2rem;
  border: 2px solid #e2e8f0;
  border-radius: 12px;
  background: white;
  cursor: pointer;
  transition: all 0.3s ease;
  min-width: 160px;
}

.tab-navigation button:hover {
  border-color: #38a169;
  background: #f0fff4;
}

.tab-navigation button.active {
  border-color: #38a169;
  background: linear-gradient(135deg, #38a169 0%, #276749 100%);
  color: white;
}

.tab-icon {
  font-size: 1.5rem;
  margin-bottom: 0.25rem;
}

.tab-label {
  font-weight: 600;
  font-size: 0.95rem;
}

.tab-desc {
  font-size: 0.75rem;
  opacity: 0.7;
  margin-top: 0.25rem;
}

/* Main Content */
main {
  flex: 1;
  padding: 2rem 1rem;
  max-width: 1200px;
  margin: 0 auto;
  width: 100%;
}

.tab-content {
  animation: fadeIn 0.3s ease;
}

@keyframes fadeIn {
  from { opacity: 0; transform: translateY(10px); }
  to { opacity: 1; transform: translateY(0); }
}

/* Intro Card */
.intro-card {
  background: white;
  border-radius: 16px;
  padding: 1.5rem 2rem;
  margin-bottom: 2rem;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
}

.intro-card h2 {
  margin: 0 0 1rem 0;
  color: #2d3748;
  font-size: 1.25rem;
}

.steps {
  display: flex;
  justify-content: center;
  gap: 2rem;
  flex-wrap: wrap;
}

.step {
  display: flex;
  align-items: center;
  gap: 0.75rem;
}

.step-num {
  width: 32px;
  height: 32px;
  background: linear-gradient(135deg, #38a169 0%, #276749 100%);
  color: white;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  font-weight: bold;
}

.step-text {
  color: #4a5568;
}

/* Container */
.container {
  max-width: 600px;
  margin: 0 auto;
}

/* Messages */
.warning-message, .error-message {
  background: #fffaf0;
  border: 1px solid #ed8936;
  border-radius: 12px;
  padding: 1.5rem;
  margin-top: 1rem;
  text-align: center;
}

.warning-message {
  background: #fffaf0;
  border-color: #ed8936;
}

.error-message {
  background: #fff5f5;
  border-color: #fc8181;
}

.warning-hint, .error-hint {
  font-size: 0.9rem;
  color: #718096;
  margin-top: 0.5rem;
}

code {
  background: #2d3748;
  color: #68d391;
  padding: 0.25rem 0.5rem;
  border-radius: 4px;
  font-family: monospace;
}

/* Footer */
footer {
  background: #2d3748;
  color: #a0aec0;
  padding: 1.5rem;
  text-align: center;
}

footer p {
  margin: 0.25rem 0;
}

.version {
  font-size: 0.8rem;
  opacity: 0.7;
}

/* Responsive */
@media (max-width: 768px) {
  .header-content h1 {
    font-size: 1.75rem;
  }
  
  .tab-navigation {
    gap: 0.5rem;
  }
  
  .tab-navigation button {
    padding: 0.75rem 1rem;
    min-width: 100px;
  }
  
  .steps {
    flex-direction: column;
    align-items: center;
  }
}
</style>
  margin: 0;
  font-size: 1.1rem;
  opacity: 0.95;
}

main {
  flex: 1;
  padding: 2rem 1rem;
  background: #f5f7fa;
}

.container {
  max-width: 900px;
  margin: 0 auto;
}

.error-message {
  background: #fee;
  border: 2px solid #fcc;
  border-radius: 12px;
  padding: 1.5rem;
  margin: 2rem 0;
  text-align: center;
}

.error-message p {
  margin: 0.5rem 0;
  color: #c33;
  font-weight: 600;
}

.error-hint {
  font-size: 0.9rem;
  color: #666;
  font-weight: 400 !important;
  margin-top: 1rem !important;
}

footer {
  background: #2d3748;
  color: white;
  text-align: center;
  padding: 1.5rem;
  font-size: 0.9rem;
}

footer p {
  margin: 0;
}

@media (max-width: 768px) {
  header h1 {
    font-size: 2rem;
  }
  
  header p {
    font-size: 1rem;
  }
}
</style>
