<script setup>
import { ref } from 'vue'

const searchQuery = ref('')
const activeCategory = ref('all')

const categories = [
  { id: 'all', name: 'All Terms', icon: '📚' },
  { id: 'training', name: 'Training', icon: '🏋️' },
  { id: 'model', name: 'Model', icon: '🧠' },
  { id: 'data', name: 'Data', icon: '📊' },
  { id: 'results', name: 'Results', icon: '📈' }
]

const glossaryTerms = [
  {
    term: 'Epoch',
    category: 'training',
    simple: 'One complete pass through all training images',
    detailed: 'Think of it like reading a textbook once from cover to cover. The model needs to read the same book multiple times (many epochs) to really learn the material. More epochs = more learning opportunities.',
    example: 'If you have 1,000 guava images and train for 30 epochs, the model will see each image 30 times.'
  },
  {
    term: 'Training Loss',
    category: 'training',
    simple: 'How many mistakes the model makes while learning',
    detailed: 'Loss measures how wrong the model\'s predictions are. Like a student taking practice tests - a high score means lots of wrong answers. We want this number to go DOWN as training progresses.',
    example: 'Loss of 0.5 is better than loss of 2.0. The model is making fewer mistakes.'
  },
  {
    term: 'Validation Loss',
    category: 'training',
    simple: 'Mistakes on images the model hasn\'t seen during training',
    detailed: 'This is the real test! Like giving a student an exam with NEW questions, not the practice questions. If the model only memorizes training images but can\'t handle new ones, this number will be high.',
    example: 'If training loss is 0.1 but validation loss is 2.0, the model is just memorizing, not actually learning.'
  },
  {
    term: 'Accuracy',
    category: 'results',
    simple: 'Percentage of correct predictions',
    detailed: 'Out of 100 guava images, if the model correctly identifies 85 of them, that\'s 85% accuracy. Higher is always better!',
    example: '95% accuracy means the model gets 95 out of 100 guavas correct.'
  },
  {
    term: 'Validation Accuracy',
    category: 'results',
    simple: 'THE MOST IMPORTANT NUMBER - accuracy on new images',
    detailed: 'This tells you how well your model will perform in the real world, on images it\'s never seen before. This is what you should focus on!',
    example: 'If validation accuracy is 90%, expect the model to be correct about 9 out of 10 times when you use it on new guava photos.'
  },
  {
    term: 'Learning Rate',
    category: 'training',
    simple: 'How fast the model learns',
    detailed: 'Like adjusting how quickly a student studies. Too fast = sloppy learning, missing details. Too slow = takes forever. The model automatically adjusts this to find the sweet spot.',
    example: 'A learning rate of 0.001 is common. If learning stalls, it might automatically drop to 0.0001.'
  },
  {
    term: 'Batch Size',
    category: 'training',
    simple: 'Number of images processed together',
    detailed: 'Instead of learning from one image at a time, the model looks at a group (batch) and learns from all of them together. Larger batches = faster training but need more memory.',
    example: 'Batch size of 32 means the model looks at 32 guava images, then updates what it learned.'
  },
  {
    term: 'Overfitting',
    category: 'training',
    simple: 'When the model memorizes instead of learning',
    detailed: 'Like a student who memorizes test answers word-for-word but doesn\'t understand the concepts. The model does great on training images but poorly on new images.',
    example: 'Training accuracy 99% but validation accuracy 60% = overfitting. The model is just memorizing!'
  },
  {
    term: 'Deep Learning',
    category: 'model',
    simple: 'AI that learns patterns from lots of examples',
    detailed: 'A type of artificial intelligence inspired by the human brain. It learns by looking at thousands of examples and finding patterns, rather than being programmed with specific rules.',
    example: 'Show it 10,000 guava images with labels, and it learns to recognize ripeness patterns on its own.'
  },
  {
    term: 'Neural Network',
    category: 'model',
    simple: 'The "brain" structure of the AI',
    detailed: 'Layers of mathematical operations loosely inspired by neurons in the brain. Information flows through these layers, getting processed and refined until a prediction comes out.',
    example: 'Our model uses ResNet50 - a neural network with 50 layers of processing.'
  },
  {
    term: 'ResNet50',
    category: 'model',
    simple: 'A powerful pre-built neural network architecture',
    detailed: 'A specific design of neural network that\'s proven to work well. The "50" means it has 50 layers. It was trained on millions of images, so it already knows a lot about recognizing things in photos.',
    example: 'Instead of starting from scratch, we use ResNet50\'s knowledge and just teach it about guavas specifically.'
  },
  {
    term: 'Transfer Learning',
    category: 'model',
    simple: 'Using knowledge from one task to help with another',
    detailed: 'Like a chef who already knows cooking techniques learning a new cuisine. They don\'t start from zero - they use existing skills. We use a model trained on millions of images and fine-tune it for guavas.',
    example: 'ResNet50 already knows about shapes, colors, and textures. We just teach it what makes a ripe vs unripe guava.'
  },
  {
    term: 'Dataset',
    category: 'data',
    simple: 'The collection of images used for training',
    detailed: 'All the guava photos organized by ripeness stage. The more images and the more variety, the better the model can learn.',
    example: 'A dataset might have 5,000 guava images: 1,000 each for day 1, day 2, day 3, day 4, and day 5.'
  },
  {
    term: 'Training Set',
    category: 'data',
    simple: 'Images the model learns from',
    detailed: 'Usually 80-90% of your dataset. These are the images the model sees during training and tries to learn from.',
    example: 'If you have 5,000 images, about 4,000 might be in the training set.'
  },
  {
    term: 'Validation Set',
    category: 'data',
    simple: 'Images used to check learning progress',
    detailed: 'A separate set of images (not used for training) that we use to see how well the model is actually learning. Like practice exams for a student.',
    example: 'About 500-1,000 images set aside just for testing during training.'
  },
  {
    term: 'Test Set',
    category: 'data',
    simple: 'Final exam images the model has never seen',
    detailed: 'Completely separate images used only at the very end to get a final score of how good the model is. Never shown during training!',
    example: 'The remaining 500 images used only once training is 100% complete.'
  },
  {
    term: 'Data Augmentation',
    category: 'data',
    simple: 'Creating variations of images to improve learning',
    detailed: 'Like practicing shooting baskets from different angles. We flip, rotate, zoom, and adjust brightness of images so the model learns to recognize guavas in any condition.',
    example: 'One guava photo becomes 10 variations: flipped, rotated, brighter, darker, zoomed in, etc.'
  },
  {
    term: 'Confidence Score',
    category: 'results',
    simple: 'How sure the model is about its prediction',
    detailed: 'A percentage showing the model\'s certainty. 95% confidence means it\'s very sure. 55% confidence means it\'s basically guessing.',
    example: 'Prediction: "Day 3 Ripe" with 92% confidence = the model is pretty sure about this!'
  },
  {
    term: 'Confusion Matrix',
    category: 'results',
    simple: 'A table showing what the model got right and wrong',
    detailed: 'A grid that shows, for each ripeness stage, how many images were correctly identified vs. mistaken for other stages. Helps identify where the model struggles.',
    example: 'The matrix might show the model often confuses Day 2 and Day 3 guavas.'
  },
  {
    term: 'Early Stopping',
    category: 'training',
    simple: 'Automatically stopping training when learning stalls',
    detailed: 'If the model stops improving for several epochs in a row, training stops automatically. This prevents wasting time and avoids overfitting.',
    example: 'If validation accuracy doesn\'t improve for 10 epochs, training stops even if we set 50 epochs.'
  },
  {
    term: 'GPU',
    category: 'model',
    simple: 'Special computer chip that speeds up AI training',
    detailed: 'Graphics Processing Units (originally for video games) are perfect for the math used in deep learning. Training that takes days on a regular computer might take hours on a GPU.',
    example: 'An NVIDIA RTX 3080 GPU can train our model 10-50x faster than using just the CPU.'
  },
  {
    term: 'Inference',
    category: 'results',
    simple: 'Using the trained model to make predictions',
    detailed: 'After training is complete, using the model to predict the ripeness of new guava images. This is the actual "using" part!',
    example: 'Upload a new guava photo → model runs inference → tells you it\'s Day 4 ripeness.'
  }
]

// Filter terms based on search and category
const filteredTerms = () => {
  return glossaryTerms.filter(item => {
    const matchesCategory = activeCategory.value === 'all' || item.category === activeCategory.value
    const matchesSearch = !searchQuery.value || 
      item.term.toLowerCase().includes(searchQuery.value.toLowerCase()) ||
      item.simple.toLowerCase().includes(searchQuery.value.toLowerCase())
    return matchesCategory && matchesSearch
  })
}
</script>

<template>
  <div class="glossary-panel">
    <div class="glossary-header">
      <h2>📖 Help & Glossary</h2>
      <p>Don't know a term? Look it up here! We explain everything in plain English.</p>
    </div>

    <!-- Search -->
    <div class="search-box">
      <span class="search-icon">🔍</span>
      <input 
        v-model="searchQuery"
        type="text" 
        placeholder="Search terms..."
        class="search-input"
      />
    </div>

    <!-- Category Filters -->
    <div class="category-filters">
      <button 
        v-for="cat in categories" 
        :key="cat.id"
        :class="{ active: activeCategory === cat.id }"
        @click="activeCategory = cat.id"
      >
        <span class="cat-icon">{{ cat.icon }}</span>
        <span class="cat-name">{{ cat.name }}</span>
      </button>
    </div>

    <!-- Quick Tips -->
    <div class="quick-tips">
      <h3>💡 Quick Tips for Non-Technical Users</h3>
      <div class="tips-grid">
        <div class="tip">
          <span class="tip-icon">📈</span>
          <div>
            <strong>Watch Validation Accuracy</strong>
            <p>This is the most important number! Higher = better model.</p>
          </div>
        </div>
        <div class="tip">
          <span class="tip-icon">📉</span>
          <div>
            <strong>Loss Should Go Down</strong>
            <p>Both training and validation loss should decrease over time.</p>
          </div>
        </div>
        <div class="tip">
          <span class="tip-icon">⚖️</span>
          <div>
            <strong>Balance is Key</strong>
            <p>Training and validation accuracy should be close to each other.</p>
          </div>
        </div>
        <div class="tip">
          <span class="tip-icon">⏱️</span>
          <div>
            <strong>Patience Pays Off</strong>
            <p>Training takes time. Let it run for at least 10-20 epochs.</p>
          </div>
        </div>
      </div>
    </div>

    <!-- Glossary Terms -->
    <div class="terms-list">
      <div 
        v-for="item in filteredTerms()" 
        :key="item.term"
        class="term-card"
      >
        <div class="term-header">
          <h3>{{ item.term }}</h3>
          <span class="term-category">{{ item.category }}</span>
        </div>
        
        <div class="term-simple">
          <span class="label">📌 In Simple Terms:</span>
          {{ item.simple }}
        </div>
        
        <div class="term-detailed">
          <span class="label">📖 Detailed Explanation:</span>
          {{ item.detailed }}
        </div>
        
        <div class="term-example">
          <span class="label">💡 Example:</span>
          {{ item.example }}
        </div>
      </div>

      <div v-if="filteredTerms().length === 0" class="no-results">
        <p>No terms found matching "{{ searchQuery }}"</p>
        <button @click="searchQuery = ''; activeCategory = 'all'">Clear filters</button>
      </div>
    </div>

    <!-- Still Confused? -->
    <div class="help-footer">
      <h3>🤔 Still Confused?</h3>
      <p>
        That's okay! Machine learning has a lot of jargon. The most important thing to remember:
      </p>
      <div class="key-points">
        <div class="key-point">
          <span>1️⃣</span>
          <span>Upload clear, well-lit photos of guavas</span>
        </div>
        <div class="key-point">
          <span>2️⃣</span>
          <span>The model will tell you how ripe it thinks the guava is</span>
        </div>
        <div class="key-point">
          <span>3️⃣</span>
          <span>Higher confidence = more reliable prediction</span>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.glossary-panel {
  max-width: 900px;
  margin: 0 auto;
}

.glossary-header {
  text-align: center;
  margin-bottom: 2rem;
}

.glossary-header h2 {
  margin: 0 0 0.5rem 0;
  color: #2d3748;
  font-size: 1.75rem;
}

.glossary-header p {
  color: #718096;
  margin: 0;
}

/* Search */
.search-box {
  display: flex;
  align-items: center;
  background: white;
  border: 2px solid #e2e8f0;
  border-radius: 12px;
  padding: 0.75rem 1rem;
  margin-bottom: 1rem;
  transition: border-color 0.2s;
}

.search-box:focus-within {
  border-color: #38a169;
}

.search-icon {
  font-size: 1.25rem;
  margin-right: 0.75rem;
}

.search-input {
  flex: 1;
  border: none;
  outline: none;
  font-size: 1rem;
  background: transparent;
}

/* Category Filters */
.category-filters {
  display: flex;
  gap: 0.5rem;
  flex-wrap: wrap;
  margin-bottom: 1.5rem;
}

.category-filters button {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  padding: 0.5rem 1rem;
  border: 2px solid #e2e8f0;
  border-radius: 20px;
  background: white;
  cursor: pointer;
  transition: all 0.2s;
  font-size: 0.9rem;
}

.category-filters button:hover {
  border-color: #38a169;
}

.category-filters button.active {
  background: #38a169;
  border-color: #38a169;
  color: white;
}

/* Quick Tips */
.quick-tips {
  background: linear-gradient(135deg, #f0fff4 0%, #e6fffa 100%);
  border-radius: 16px;
  padding: 1.5rem;
  margin-bottom: 2rem;
  border: 1px solid #9ae6b4;
}

.quick-tips h3 {
  margin: 0 0 1rem 0;
  color: #276749;
}

.tips-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
  gap: 1rem;
}

.tip {
  display: flex;
  gap: 0.75rem;
  background: white;
  padding: 1rem;
  border-radius: 12px;
}

.tip-icon {
  font-size: 1.5rem;
}

.tip strong {
  display: block;
  color: #2d3748;
  margin-bottom: 0.25rem;
}

.tip p {
  margin: 0;
  font-size: 0.85rem;
  color: #718096;
}

/* Terms List */
.terms-list {
  display: flex;
  flex-direction: column;
  gap: 1rem;
  margin-bottom: 2rem;
}

.term-card {
  background: white;
  border-radius: 16px;
  padding: 1.5rem;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
  border-left: 4px solid #38a169;
}

.term-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 1rem;
}

.term-header h3 {
  margin: 0;
  color: #2d3748;
  font-size: 1.25rem;
}

.term-category {
  background: #e2e8f0;
  padding: 0.25rem 0.75rem;
  border-radius: 12px;
  font-size: 0.75rem;
  text-transform: uppercase;
  color: #718096;
}

.term-simple, .term-detailed, .term-example {
  margin-bottom: 1rem;
  padding: 0.75rem;
  border-radius: 8px;
  line-height: 1.6;
}

.term-simple {
  background: #f0fff4;
  color: #276749;
  font-weight: 500;
}

.term-detailed {
  background: #f7fafc;
  color: #4a5568;
}

.term-example {
  background: #fffaf0;
  color: #744210;
  font-style: italic;
  margin-bottom: 0;
}

.label {
  display: block;
  font-weight: 600;
  margin-bottom: 0.5rem;
  font-size: 0.85rem;
}

/* No Results */
.no-results {
  text-align: center;
  padding: 3rem;
  background: #f7fafc;
  border-radius: 16px;
}

.no-results button {
  margin-top: 1rem;
  padding: 0.5rem 1rem;
  background: #38a169;
  color: white;
  border: none;
  border-radius: 8px;
  cursor: pointer;
}

/* Help Footer */
.help-footer {
  background: #2d3748;
  color: white;
  border-radius: 16px;
  padding: 2rem;
  text-align: center;
}

.help-footer h3 {
  margin: 0 0 1rem 0;
}

.help-footer > p {
  color: #a0aec0;
  margin-bottom: 1.5rem;
}

.key-points {
  display: flex;
  flex-direction: column;
  gap: 0.75rem;
  max-width: 400px;
  margin: 0 auto;
}

.key-point {
  display: flex;
  align-items: center;
  gap: 1rem;
  background: rgba(255, 255, 255, 0.1);
  padding: 0.75rem 1rem;
  border-radius: 8px;
  text-align: left;
}

.key-point span:first-child {
  font-size: 1.25rem;
}

@media (max-width: 768px) {
  .tips-grid {
    grid-template-columns: 1fr;
  }
  
  .category-filters {
    justify-content: center;
  }
}
</style>
