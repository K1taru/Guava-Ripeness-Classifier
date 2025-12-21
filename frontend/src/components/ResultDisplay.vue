<script setup>
defineProps({
  prediction: {
    type: Object,
    required: true
  }
})

const getConfidenceColor = (confidence) => {
  if (confidence >= 0.8) return '#48bb78'  // Green
  if (confidence >= 0.6) return '#ed8936'  // Orange
  return '#fc8181'  // Red
}

const getConfidenceLabel = (confidence) => {
  if (confidence >= 0.8) return 'High Confidence'
  if (confidence >= 0.6) return 'Medium Confidence'
  return 'Low Confidence'
}
</script>

<template>
  <div class="result-card">
    <div class="result-header">
      <h2>✨ Prediction Results</h2>
    </div>

    <div class="result-content">
      <!-- Main Prediction -->
      <div class="main-result">
        <div class="result-label">Ripeness Stage</div>
        <div class="result-value">
          {{ prediction.predicted_class || prediction.class }}
        </div>
      </div>

      <!-- Confidence Score -->
      <div class="confidence-section">
        <div class="confidence-label">
          Confidence Score
        </div>
        <div class="confidence-bar-container">
          <div 
            class="confidence-bar"
            :style="{
              width: `${(prediction.confidence || 0) * 100}%`,
              backgroundColor: getConfidenceColor(prediction.confidence || 0)
            }"
          >
          </div>
        </div>
        <div class="confidence-details">
          <span class="confidence-percent">
            {{ ((prediction.confidence || 0) * 100).toFixed(1) }}%
          </span>
          <span 
            class="confidence-status"
            :style="{ color: getConfidenceColor(prediction.confidence || 0) }"
          >
            {{ getConfidenceLabel(prediction.confidence || 0) }}
          </span>
        </div>
      </div>

      <!-- Top Predictions (if available) -->
      <div v-if="prediction.top_predictions" class="top-predictions">
        <h3>Top Predictions</h3>
        <div 
          v-for="(pred, index) in prediction.top_predictions.slice(0, 5)" 
          :key="index"
          class="prediction-item"
        >
          <div class="pred-rank">{{ index + 1 }}</div>
          <div class="pred-class">{{ pred.class }}</div>
          <div class="pred-confidence">{{ (pred.confidence * 100).toFixed(1) }}%</div>
        </div>
      </div>

      <!-- Additional Info -->
      <div v-if="prediction.processing_time" class="meta-info">
        <div class="meta-item">
          <span class="meta-label">Processing Time:</span>
          <span class="meta-value">{{ prediction.processing_time.toFixed(3) }}s</span>
        </div>
      </div>

      <div v-if="prediction.model_name" class="meta-info">
        <div class="meta-item">
          <span class="meta-label">Model:</span>
          <span class="meta-value">{{ prediction.model_name }}</span>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.result-card {
  background: white;
  border-radius: 16px;
  padding: 2rem;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.07);
  margin-top: 2rem;
  animation: slideUp 0.4s ease-out;
}

@keyframes slideUp {
  from {
    opacity: 0;
    transform: translateY(20px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

.result-header {
  margin-bottom: 1.5rem;
  padding-bottom: 1rem;
  border-bottom: 2px solid #e2e8f0;
}

.result-header h2 {
  margin: 0;
  color: #2d3748;
  font-size: 1.5rem;
}

.result-content {
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.main-result {
  text-align: center;
  padding: 2rem;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  border-radius: 12px;
  color: white;
}

.result-label {
  font-size: 0.9rem;
  text-transform: uppercase;
  letter-spacing: 1px;
  opacity: 0.9;
  margin-bottom: 0.5rem;
}

.result-value {
  font-size: 2.5rem;
  font-weight: 700;
  margin: 0;
}

.confidence-section {
  padding: 1.5rem;
  background: #f7fafc;
  border-radius: 12px;
}

.confidence-label {
  font-weight: 600;
  color: #4a5568;
  margin-bottom: 0.75rem;
}

.confidence-bar-container {
  width: 100%;
  height: 24px;
  background: #e2e8f0;
  border-radius: 12px;
  overflow: hidden;
  margin-bottom: 0.75rem;
}

.confidence-bar {
  height: 100%;
  transition: width 0.6s ease-out, background-color 0.3s ease;
  border-radius: 12px;
  position: relative;
}

.confidence-details {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.confidence-percent {
  font-size: 1.5rem;
  font-weight: 700;
  color: #2d3748;
}

.confidence-status {
  font-size: 0.9rem;
  font-weight: 600;
}

.top-predictions {
  padding: 1.5rem;
  background: #f7fafc;
  border-radius: 12px;
}

.top-predictions h3 {
  margin: 0 0 1rem 0;
  font-size: 1.1rem;
  color: #4a5568;
}

.prediction-item {
  display: grid;
  grid-template-columns: 40px 1fr auto;
  gap: 1rem;
  padding: 0.75rem;
  background: white;
  border-radius: 8px;
  margin-bottom: 0.5rem;
  align-items: center;
}

.prediction-item:last-child {
  margin-bottom: 0;
}

.pred-rank {
  width: 32px;
  height: 32px;
  background: #667eea;
  color: white;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  font-weight: 700;
  font-size: 0.9rem;
}

.pred-class {
  font-weight: 600;
  color: #2d3748;
}

.pred-confidence {
  color: #718096;
  font-weight: 600;
}

.meta-info {
  padding: 1rem;
  background: #f7fafc;
  border-radius: 8px;
}

.meta-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.meta-label {
  color: #718096;
  font-weight: 500;
}

.meta-value {
  color: #2d3748;
  font-weight: 600;
}

@media (max-width: 768px) {
  .result-card {
    padding: 1.5rem;
  }

  .result-value {
    font-size: 2rem;
  }

  .confidence-percent {
    font-size: 1.25rem;
  }
}
</style>
