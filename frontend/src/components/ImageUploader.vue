<script setup>
import { ref } from 'vue'

defineProps({
  loading: Boolean
})

const emit = defineEmits(['predict', 'reset'])

const selectedImage = ref(null)
const previewUrl = ref(null)
const isDragging = ref(false)

const handleFileSelect = (event) => {
  const file = event.target.files[0]
  if (file && file.type.startsWith('image/')) {
    processFile(file)
  }
}

const handleDrop = (event) => {
  event.preventDefault()
  isDragging.value = false
  
  const file = event.dataTransfer.files[0]
  if (file && file.type.startsWith('image/')) {
    processFile(file)
  }
}

const handleDragOver = (event) => {
  event.preventDefault()
  isDragging.value = true
}

const handleDragLeave = () => {
  isDragging.value = false
}

const processFile = (file) => {
  selectedImage.value = file
  
  const reader = new FileReader()
  reader.onload = (e) => {
    previewUrl.value = e.target.result
  }
  reader.readAsDataURL(file)
}

const handlePredict = () => {
  if (selectedImage.value) {
    emit('predict', selectedImage.value)
  }
}

const handleReset = () => {
  selectedImage.value = null
  previewUrl.value = null
  emit('reset')
}
</script>

<template>
  <div class="uploader-card">
    <h2>Upload Guava Image</h2>
    
    <div 
      class="drop-zone"
      :class="{ 'dragging': isDragging, 'has-image': previewUrl }"
      @drop="handleDrop"
      @dragover="handleDragOver"
      @dragleave="handleDragLeave"
    >
      <div v-if="!previewUrl" class="upload-prompt">
        <div class="upload-icon">📸</div>
        <p class="upload-text">Drag & drop or click to select</p>
        <p class="upload-hint">Supports: JPG, PNG, WebP</p>
        <input 
          type="file" 
          accept="image/*"
          @change="handleFileSelect"
          class="file-input"
          id="file-input"
        />
        <label for="file-input" class="upload-button">
          Choose Image
        </label>
      </div>

      <div v-else class="preview-container">
        <img :src="previewUrl" alt="Selected guava" class="preview-image" />
        <button @click="handleReset" class="remove-button" :disabled="loading">
          ✕ Remove
        </button>
      </div>
    </div>

    <div v-if="previewUrl" class="action-buttons">
      <button 
        @click="handlePredict" 
        class="predict-button"
        :disabled="loading"
      >
        <span v-if="!loading">🔍 Analyze Ripeness</span>
        <span v-else class="loading-spinner">
          <span class="spinner"></span> Analyzing...
        </span>
      </button>
    </div>
  </div>
</template>

<style scoped>
.uploader-card {
  background: white;
  border-radius: 16px;
  padding: 2rem;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.07);
}

h2 {
  margin: 0 0 1.5rem 0;
  color: #2d3748;
  font-size: 1.5rem;
}

.drop-zone {
  border: 3px dashed #cbd5e0;
  border-radius: 12px;
  padding: 2rem;
  text-align: center;
  transition: all 0.3s ease;
  min-height: 300px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.drop-zone.dragging {
  border-color: #667eea;
  background: #f0f4ff;
}

.drop-zone.has-image {
  border-style: solid;
  border-color: #48bb78;
  padding: 1rem;
}

.upload-prompt {
  width: 100%;
}

.upload-icon {
  font-size: 4rem;
  margin-bottom: 1rem;
}

.upload-text {
  font-size: 1.2rem;
  color: #4a5568;
  margin: 0.5rem 0;
  font-weight: 600;
}

.upload-hint {
  color: #718096;
  font-size: 0.9rem;
  margin: 0.5rem 0 1.5rem 0;
}

.file-input {
  display: none;
}

.upload-button {
  display: inline-block;
  padding: 0.75rem 2rem;
  background: #667eea;
  color: white;
  border-radius: 8px;
  cursor: pointer;
  font-weight: 600;
  transition: all 0.2s ease;
}

.upload-button:hover {
  background: #5568d3;
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(102, 126, 234, 0.4);
}

.preview-container {
  width: 100%;
  position: relative;
}

.preview-image {
  max-width: 100%;
  max-height: 400px;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
}

.remove-button {
  margin-top: 1rem;
  padding: 0.5rem 1.5rem;
  background: #fc8181;
  color: white;
  border: none;
  border-radius: 6px;
  cursor: pointer;
  font-weight: 600;
  transition: all 0.2s ease;
}

.remove-button:hover:not(:disabled) {
  background: #f56565;
}

.remove-button:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.action-buttons {
  margin-top: 1.5rem;
  display: flex;
  justify-content: center;
}

.predict-button {
  padding: 1rem 3rem;
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  border: none;
  border-radius: 8px;
  font-size: 1.1rem;
  font-weight: 700;
  cursor: pointer;
  transition: all 0.3s ease;
  box-shadow: 0 4px 15px rgba(102, 126, 234, 0.4);
}

.predict-button:hover:not(:disabled) {
  transform: translateY(-2px);
  box-shadow: 0 6px 20px rgba(102, 126, 234, 0.5);
}

.predict-button:disabled {
  opacity: 0.7;
  cursor: not-allowed;
  transform: none;
}

.loading-spinner {
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 0.5rem;
}

.spinner {
  display: inline-block;
  width: 16px;
  height: 16px;
  border: 3px solid rgba(255, 255, 255, 0.3);
  border-radius: 50%;
  border-top-color: white;
  animation: spin 0.8s linear infinite;
}

@keyframes spin {
  to {
    transform: rotate(360deg);
  }
}

@media (max-width: 768px) {
  .uploader-card {
    padding: 1.5rem;
  }

  .drop-zone {
    min-height: 250px;
    padding: 1.5rem;
  }

  .upload-icon {
    font-size: 3rem;
  }

  .predict-button {
    width: 100%;
    padding: 0.875rem 2rem;
  }
}
</style>
