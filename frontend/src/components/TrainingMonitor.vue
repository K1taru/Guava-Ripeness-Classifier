<script setup>
import { ref, computed, watch } from 'vue'

const props = defineProps({
  status: {
    type: Object,
    required: true
  },
  connected: Boolean,
  backendUrl: String
})

const emit = defineEmits(['refresh'])

// Expanded sections
const expandedSections = ref({
  metrics: true,
  logs: true,
  chart: true
})

// Computed values with explanations
const progressPercent = computed(() => {
  if (!props.status.total_epochs) return 0
  return (props.status.current_epoch / props.status.total_epochs) * 100
})

const epochDisplay = computed(() => {
  return `${props.status.current_epoch || 0} / ${props.status.total_epochs || 0}`
})

// Format numbers nicely
const formatNumber = (num, decimals = 4) => {
  if (num === null || num === undefined) return '—'
  return Number(num).toFixed(decimals)
}

const formatPercent = (num) => {
  if (num === null || num === undefined) return '—'
  return `${Number(num).toFixed(2)}%`
}

// Get status color
const getStatusColor = (value, type) => {
  if (type === 'loss') {
    if (value < 0.5) return '#48bb78'
    if (value < 1.0) return '#ed8936'
    return '#fc8181'
  }
  if (type === 'accuracy') {
    if (value >= 90) return '#48bb78'
    if (value >= 70) return '#ed8936'
    return '#fc8181'
  }
  return '#718096'
}

// Toggle section
const toggleSection = (section) => {
  expandedSections.value[section] = !expandedSections.value[section]
}
</script>

<template>
  <div class="training-monitor">
    <!-- Header -->
    <div class="monitor-header">
      <h2>📊 Training Monitor</h2>
      <p class="subtitle">Watch your AI model learn in real-time</p>
      <button @click="emit('refresh')" class="refresh-btn" :disabled="!connected">
        🔄 Refresh
      </button>
    </div>

    <!-- Connection Warning -->
    <div v-if="!connected" class="offline-warning">
      <div class="warning-icon">⚠️</div>
      <div class="warning-content">
        <h3>Backend Not Connected</h3>
        <p>The training monitor requires the Python backend to be running.</p>
        <p class="hint">
          <strong>What this means:</strong> The backend server handles all the heavy computation 
          (training the AI model). Without it, we can't show you the training progress.
        </p>
        <code>cd backend && python app.py</code>
      </div>
    </div>

    <!-- Training Status -->
    <div v-else class="monitor-content">
      <!-- Status Banner -->
      <div class="status-banner" :class="{ training: status.is_training }">
        <div class="status-indicator">
          <span class="status-dot" :class="{ active: status.is_training }"></span>
          <span class="status-text">
            {{ status.is_training ? '🏃 Training in Progress' : '⏸️ Not Training' }}
          </span>
        </div>
        <div v-if="status.is_training" class="progress-section">
          <div class="progress-label">
            <span>Epoch {{ epochDisplay }}</span>
            <span class="progress-percent">{{ progressPercent.toFixed(0) }}%</span>
          </div>
          <div class="progress-bar">
            <div class="progress-fill" :style="{ width: `${progressPercent}%` }"></div>
          </div>
        </div>
      </div>

      <!-- Quick Explanation -->
      <div class="explainer-card">
        <h3>📖 What am I looking at?</h3>
        <p>
          This panel shows how well your AI model is learning to identify guava ripeness stages. 
          The model looks at thousands of guava images and gradually improves its accuracy.
        </p>
      </div>

      <!-- Metrics Section -->
      <div class="section" :class="{ collapsed: !expandedSections.metrics }">
        <div class="section-header" @click="toggleSection('metrics')">
          <h3>📈 Performance Metrics</h3>
          <span class="toggle-icon">{{ expandedSections.metrics ? '▼' : '▶' }}</span>
        </div>
        
        <div v-show="expandedSections.metrics" class="section-content">
          <div class="metrics-grid">
            <!-- Training Loss -->
            <div class="metric-card">
              <div class="metric-header">
                <span class="metric-icon">📉</span>
                <span class="metric-name">Training Loss</span>
                <span class="metric-help" title="Lower is better. Shows how many mistakes the model makes on training data.">❓</span>
              </div>
              <div class="metric-value" :style="{ color: getStatusColor(status.train_loss, 'loss') }">
                {{ formatNumber(status.train_loss) }}
              </div>
              <div class="metric-explain">
                <strong>What it means:</strong> How many mistakes the model makes while learning. 
                Lower numbers = fewer mistakes = better learning!
              </div>
            </div>

            <!-- Validation Loss -->
            <div class="metric-card">
              <div class="metric-header">
                <span class="metric-icon">📊</span>
                <span class="metric-name">Validation Loss</span>
                <span class="metric-help" title="Lower is better. Shows how well the model handles NEW images it hasn't seen.">❓</span>
              </div>
              <div class="metric-value" :style="{ color: getStatusColor(status.val_loss, 'loss') }">
                {{ formatNumber(status.val_loss) }}
              </div>
              <div class="metric-explain">
                <strong>What it means:</strong> How well the model handles NEW images it hasn't seen before. 
                This is the real test of learning!
              </div>
            </div>

            <!-- Training Accuracy -->
            <div class="metric-card">
              <div class="metric-header">
                <span class="metric-icon">🎯</span>
                <span class="metric-name">Training Accuracy</span>
                <span class="metric-help" title="Higher is better. Percentage of correct predictions on training images.">❓</span>
              </div>
              <div class="metric-value" :style="{ color: getStatusColor(status.train_acc, 'accuracy') }">
                {{ formatPercent(status.train_acc) }}
              </div>
              <div class="metric-explain">
                <strong>What it means:</strong> How often the model correctly identifies guava ripeness 
                in the images it's learning from. Higher = better!
              </div>
            </div>

            <!-- Validation Accuracy -->
            <div class="metric-card highlight">
              <div class="metric-header">
                <span class="metric-icon">⭐</span>
                <span class="metric-name">Validation Accuracy</span>
                <span class="metric-help" title="THE MOST IMPORTANT NUMBER! Shows real-world performance.">❓</span>
              </div>
              <div class="metric-value" :style="{ color: getStatusColor(status.val_acc, 'accuracy') }">
                {{ formatPercent(status.val_acc) }}
              </div>
              <div class="metric-explain">
                <strong>⭐ MOST IMPORTANT!</strong> This shows how accurate the model is on images 
                it's never seen. This is what really matters for real-world use!
              </div>
            </div>

            <!-- Best Accuracy -->
            <div class="metric-card">
              <div class="metric-header">
                <span class="metric-icon">🏆</span>
                <span class="metric-name">Best Accuracy So Far</span>
              </div>
              <div class="metric-value best">
                {{ formatPercent(status.best_val_acc) }}
              </div>
              <div class="metric-explain">
                The highest accuracy the model has achieved during training. 
                The model is automatically saved when it beats this record!
              </div>
            </div>

            <!-- Learning Rate -->
            <div class="metric-card">
              <div class="metric-header">
                <span class="metric-icon">⚡</span>
                <span class="metric-name">Learning Rate</span>
              </div>
              <div class="metric-value">
                {{ status.learning_rate ? status.learning_rate.toExponential(2) : '—' }}
              </div>
              <div class="metric-explain">
                How fast the model learns. Too fast = messy learning. Too slow = takes forever. 
                This adjusts automatically!
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- Training Logs Section -->
      <div class="section" :class="{ collapsed: !expandedSections.logs }">
        <div class="section-header" @click="toggleSection('logs')">
          <h3>📝 Training Logs</h3>
          <span class="toggle-icon">{{ expandedSections.logs ? '▼' : '▶' }}</span>
        </div>
        
        <div v-show="expandedSections.logs" class="section-content">
          <div class="logs-container">
            <div v-if="!status.logs || status.logs.length === 0" class="no-logs">
              <p>No training logs yet. Start training to see progress here!</p>
            </div>
            <div v-else class="logs-list">
              <div 
                v-for="(log, index) in status.logs.slice(-20).reverse()" 
                :key="index"
                class="log-entry"
                :class="log.type || 'info'"
              >
                <span class="log-time">{{ log.timestamp || '' }}</span>
                <span class="log-message">{{ log.message }}</span>
              </div>
            </div>
          </div>
          <p class="logs-hint">
            💡 These logs show what's happening during training, just like the outputs 
            you'd see in Jupyter Notebook!
          </p>
        </div>
      </div>

      <!-- Interpretation Guide -->
      <div class="interpretation-guide">
        <h3>🎓 How to Read These Numbers</h3>
        <div class="guide-grid">
          <div class="guide-item good">
            <span class="guide-icon">✅</span>
            <div>
              <strong>Good Signs</strong>
              <ul>
                <li>Validation accuracy going UP</li>
                <li>Both losses going DOWN</li>
                <li>Train and Val accuracy are close</li>
              </ul>
            </div>
          </div>
          <div class="guide-item warning">
            <span class="guide-icon">⚠️</span>
            <div>
              <strong>Warning Signs</strong>
              <ul>
                <li>Validation loss going UP while training loss goes down (overfitting)</li>
                <li>Accuracy stuck at same number for many epochs</li>
                <li>Big gap between training and validation accuracy</li>
              </ul>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.training-monitor {
  max-width: 1000px;
  margin: 0 auto;
}

.monitor-header {
  text-align: center;
  margin-bottom: 2rem;
}

.monitor-header h2 {
  margin: 0 0 0.5rem 0;
  color: #2d3748;
  font-size: 1.75rem;
}

.subtitle {
  color: #718096;
  margin: 0 0 1rem 0;
}

.refresh-btn {
  padding: 0.5rem 1rem;
  background: #38a169;
  color: white;
  border: none;
  border-radius: 8px;
  cursor: pointer;
  font-size: 0.9rem;
  transition: all 0.2s;
}

.refresh-btn:hover:not(:disabled) {
  background: #276749;
}

.refresh-btn:disabled {
  background: #a0aec0;
  cursor: not-allowed;
}

/* Offline Warning */
.offline-warning {
  background: #fffaf0;
  border: 2px solid #ed8936;
  border-radius: 16px;
  padding: 2rem;
  display: flex;
  gap: 1.5rem;
  align-items: flex-start;
}

.warning-icon {
  font-size: 3rem;
}

.warning-content h3 {
  margin: 0 0 0.5rem 0;
  color: #c05621;
}

.warning-content p {
  margin: 0.5rem 0;
  color: #744210;
}

.warning-content .hint {
  background: #fefcbf;
  padding: 1rem;
  border-radius: 8px;
  margin-top: 1rem;
}

.warning-content code {
  display: block;
  margin-top: 1rem;
  padding: 0.75rem;
  background: #2d3748;
  color: #68d391;
  border-radius: 8px;
  font-family: monospace;
}

/* Status Banner */
.status-banner {
  background: white;
  border-radius: 16px;
  padding: 1.5rem;
  margin-bottom: 1.5rem;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
  border-left: 4px solid #a0aec0;
}

.status-banner.training {
  border-left-color: #48bb78;
  background: linear-gradient(90deg, #f0fff4 0%, white 30%);
}

.status-indicator {
  display: flex;
  align-items: center;
  gap: 0.75rem;
  margin-bottom: 1rem;
}

.status-dot {
  width: 12px;
  height: 12px;
  border-radius: 50%;
  background: #a0aec0;
}

.status-dot.active {
  background: #48bb78;
  animation: pulse 1.5s infinite;
}

.status-text {
  font-size: 1.1rem;
  font-weight: 600;
  color: #2d3748;
}

.progress-section {
  margin-top: 1rem;
}

.progress-label {
  display: flex;
  justify-content: space-between;
  margin-bottom: 0.5rem;
  font-size: 0.9rem;
  color: #4a5568;
}

.progress-bar {
  height: 8px;
  background: #e2e8f0;
  border-radius: 4px;
  overflow: hidden;
}

.progress-fill {
  height: 100%;
  background: linear-gradient(90deg, #48bb78, #38a169);
  border-radius: 4px;
  transition: width 0.5s ease;
}

/* Explainer Card */
.explainer-card {
  background: #ebf8ff;
  border-radius: 12px;
  padding: 1.25rem;
  margin-bottom: 1.5rem;
  border-left: 4px solid #4299e1;
}

.explainer-card h3 {
  margin: 0 0 0.5rem 0;
  color: #2b6cb0;
  font-size: 1rem;
}

.explainer-card p {
  margin: 0;
  color: #2c5282;
  font-size: 0.95rem;
  line-height: 1.6;
}

/* Sections */
.section {
  background: white;
  border-radius: 16px;
  margin-bottom: 1.5rem;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
  overflow: hidden;
}

.section-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 1.25rem 1.5rem;
  cursor: pointer;
  background: #f7fafc;
  border-bottom: 1px solid #e2e8f0;
  transition: background 0.2s;
}

.section-header:hover {
  background: #edf2f7;
}

.section-header h3 {
  margin: 0;
  color: #2d3748;
  font-size: 1.1rem;
}

.toggle-icon {
  color: #718096;
  font-size: 0.8rem;
}

.section-content {
  padding: 1.5rem;
}

/* Metrics Grid */
.metrics-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
  gap: 1rem;
}

.metric-card {
  background: #f7fafc;
  border-radius: 12px;
  padding: 1.25rem;
  border: 1px solid #e2e8f0;
}

.metric-card.highlight {
  background: #f0fff4;
  border-color: #48bb78;
}

.metric-header {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  margin-bottom: 0.75rem;
}

.metric-icon {
  font-size: 1.25rem;
}

.metric-name {
  font-weight: 600;
  color: #2d3748;
  flex: 1;
}

.metric-help {
  cursor: help;
  opacity: 0.6;
}

.metric-value {
  font-size: 2rem;
  font-weight: 700;
  margin-bottom: 0.75rem;
}

.metric-value.best {
  color: #d69e2e;
}

.metric-explain {
  font-size: 0.85rem;
  color: #718096;
  line-height: 1.5;
  padding-top: 0.75rem;
  border-top: 1px dashed #e2e8f0;
}

/* Logs */
.logs-container {
  background: #1a202c;
  border-radius: 8px;
  padding: 1rem;
  max-height: 300px;
  overflow-y: auto;
}

.no-logs {
  color: #a0aec0;
  text-align: center;
  padding: 2rem;
}

.logs-list {
  font-family: monospace;
  font-size: 0.85rem;
}

.log-entry {
  padding: 0.5rem;
  border-bottom: 1px solid #2d3748;
  display: flex;
  gap: 1rem;
}

.log-entry.info { color: #68d391; }
.log-entry.warning { color: #ed8936; }
.log-entry.error { color: #fc8181; }
.log-entry.success { color: #48bb78; }

.log-time {
  color: #718096;
  min-width: 80px;
}

.logs-hint {
  font-size: 0.85rem;
  color: #718096;
  margin-top: 1rem;
  padding: 0.75rem;
  background: #f7fafc;
  border-radius: 8px;
}

/* Interpretation Guide */
.interpretation-guide {
  background: white;
  border-radius: 16px;
  padding: 1.5rem;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
}

.interpretation-guide h3 {
  margin: 0 0 1rem 0;
  color: #2d3748;
}

.guide-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
  gap: 1rem;
}

.guide-item {
  display: flex;
  gap: 1rem;
  padding: 1rem;
  border-radius: 12px;
}

.guide-item.good {
  background: #f0fff4;
  border: 1px solid #c6f6d5;
}

.guide-item.warning {
  background: #fffaf0;
  border: 1px solid #feebc8;
}

.guide-icon {
  font-size: 1.5rem;
}

.guide-item strong {
  display: block;
  margin-bottom: 0.5rem;
  color: #2d3748;
}

.guide-item ul {
  margin: 0;
  padding-left: 1.25rem;
  color: #4a5568;
  font-size: 0.9rem;
}

.guide-item li {
  margin-bottom: 0.25rem;
}

@keyframes pulse {
  0%, 100% { opacity: 1; transform: scale(1); }
  50% { opacity: 0.7; transform: scale(1.1); }
}

@media (max-width: 768px) {
  .metrics-grid {
    grid-template-columns: 1fr;
  }
  
  .offline-warning {
    flex-direction: column;
    text-align: center;
  }
}
</style>
