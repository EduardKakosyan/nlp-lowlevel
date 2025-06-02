'use client'

import React, { useState, useRef, useEffect, useCallback } from 'react'
import { motion, AnimatePresence, useMotionValue, useSpring, useAnimation } from 'framer-motion'
import { Card } from '@/components/ui/card'
import { Badge } from '@/components/ui/badge'
import { Button } from '@/components/ui/button'
import { 
  Star, 
  ArrowUpRight, 
  Download, 
  Heart, 
  Share2, 
  X, 
  ChevronLeft, 
  ChevronRight,
  Grid3x3,
  Maximize2,
  Filter,
  Search,
  MoreHorizontal,
  Zap
} from 'lucide-react'

interface ImageData {
  id: string
  src: string
  alt: string
  category?: string
  tags?: string[]
  likes?: number
  downloads?: number
}

const DEMO_IMAGES: ImageData[] = [
  {
    id: '1',
    src: 'https://images.unsplash.com/photo-1558618047-3c8c76ca7d13?w=800&h=600&fit=crop',
    alt: 'Person reaching up in cosmic scene',
    category: 'Portrait',
    tags: ['cosmic', 'dramatic', 'portrait'],
    likes: 234,
    downloads: 45
  },
  {
    id: '2', 
    src: 'https://images.unsplash.com/photo-1506905925346-21bda4d32df4?w=800&h=600&fit=crop',
    alt: 'Mountain landscape at golden hour',
    category: 'Landscape',
    tags: ['mountain', 'golden hour', 'nature'],
    likes: 156,
    downloads: 78
  },
  {
    id: '3',
    src: 'https://images.unsplash.com/photo-1441974231531-c6227db76b6e?w=800&h=600&fit=crop',
    alt: 'Forest path with morning light',
    category: 'Nature',
    tags: ['forest', 'path', 'morning'],
    likes: 289,
    downloads: 123
  },
  {
    id: '4',
    src: 'https://images.unsplash.com/photo-1469474968028-56623f02e42e?w=800&h=600&fit=crop',
    alt: 'Mountain lake reflection',
    category: 'Landscape',
    tags: ['lake', 'reflection', 'serene'],
    likes: 342,
    downloads: 89
  },
  {
    id: '5',
    src: 'https://images.unsplash.com/photo-1519904981063-b0cf448d479e?w=800&h=600&fit=crop',
    alt: 'Mountain peaks in winter',
    category: 'Nature',
    tags: ['winter', 'peaks', 'snow'],
    likes: 198,
    downloads: 67
  },
  {
    id: '6',
    src: 'https://images.unsplash.com/photo-1511671782779-c97d3d27a1d4?w=800&h=600&fit=crop',
    alt: 'Recording studio microphone',
    category: 'Music',
    tags: ['studio', 'microphone', 'audio'],
    likes: 445,
    downloads: 156
  },
  {
    id: '7',
    src: 'https://images.unsplash.com/photo-1518837695005-2083093ee35b?w=800&h=600&fit=crop',
    alt: 'Ocean waves at sunset',
    category: 'Seascape',
    tags: ['ocean', 'waves', 'sunset'],
    likes: 567,
    downloads: 234
  },
  {
    id: '8',
    src: 'https://images.unsplash.com/photo-1551698618-1dfe5d97d256?w=800&h=600&fit=crop',
    alt: 'Portrait with blue tones',
    category: 'Portrait',
    tags: ['portrait', 'blue', 'mood'],
    likes: 389,
    downloads: 112
  },
  {
    id: '9',
    src: 'https://images.unsplash.com/photo-1506905925346-21bda4d32df4?w=800&h=600&fit=crop',
    alt: 'Desert landscape with cacti',
    category: 'Desert',
    tags: ['desert', 'cacti', 'arid'],
    likes: 276,
    downloads: 91
  },
  {
    id: '10',
    src: 'https://images.unsplash.com/photo-1558618047-3c8c76ca7d13?w=800&h=600&fit=crop',
    alt: 'Urban architecture at night',
    category: 'Architecture',
    tags: ['urban', 'night', 'modern'],
    likes: 423,
    downloads: 187
  }
]

export default function StudioShotsGalleryEnhanced() {
  const [hoveredId, setHoveredId] = useState<string | null>(null)
  const [selectedImages, setSelectedImages] = useState<Set<string>>(new Set())
  const [fullscreenImage, setFullscreenImage] = useState<string | null>(null)
  const [viewMode, setViewMode] = useState<'grid' | 'masonry' | 'list'>('grid')
  const [isPublished, setIsPublished] = useState(true)
  const [searchTerm, setSearchTerm] = useState('')
  const [selectedCategory, setSelectedCategory] = useState<string | null>(null)
  const [sortBy, setSortBy] = useState<'recent' | 'likes' | 'downloads'>('recent')
  
  const containerRef = useRef<HTMLDivElement>(null)
  const controls = useAnimation()
  
  const mouseX = useMotionValue(0)
  const mouseY = useMotionValue(0)
  const springConfig = { damping: 25, stiffness: 700 }
  const x = useSpring(mouseX, springConfig)
  const y = useSpring(mouseY, springConfig)

  // Keyboard navigation
  useEffect(() => {
    const handleKeyDown = (e: KeyboardEvent) => {
      if (fullscreenImage) {
        if (e.key === 'Escape') {
          setFullscreenImage(null)
        } else if (e.key === 'ArrowLeft') {
          const currentIndex = DEMO_IMAGES.findIndex(img => img.id === fullscreenImage)
          const prevIndex = (currentIndex - 1 + DEMO_IMAGES.length) % DEMO_IMAGES.length
          setFullscreenImage(DEMO_IMAGES[prevIndex].id)
        } else if (e.key === 'ArrowRight') {
          const currentIndex = DEMO_IMAGES.findIndex(img => img.id === fullscreenImage)
          const nextIndex = (currentIndex + 1) % DEMO_IMAGES.length
          setFullscreenImage(DEMO_IMAGES[nextIndex].id)
        }
      }
    }

    document.addEventListener('keydown', handleKeyDown)
    return () => document.removeEventListener('keydown', handleKeyDown)
  }, [fullscreenImage])

  // Mouse tracking
  useEffect(() => {
    const handleMouseMove = (e: MouseEvent) => {
      if (containerRef.current) {
        const rect = containerRef.current.getBoundingClientRect()
        mouseX.set(e.clientX - rect.left)
        mouseY.set(e.clientY - rect.top)
      }
    }

    const container = containerRef.current
    if (container) {
      container.addEventListener('mousemove', handleMouseMove)
      return () => container.removeEventListener('mousemove', handleMouseMove)
    }
  }, [mouseX, mouseY])

  // Filter and sort images
  const filteredImages = DEMO_IMAGES
    .filter(image => {
      const matchesSearch = searchTerm === '' || 
        image.alt.toLowerCase().includes(searchTerm.toLowerCase()) ||
        image.tags?.some(tag => tag.toLowerCase().includes(searchTerm.toLowerCase()))
      const matchesCategory = !selectedCategory || image.category === selectedCategory
      return matchesSearch && matchesCategory
    })
    .sort((a, b) => {
      switch (sortBy) {
        case 'likes': return (b.likes || 0) - (a.likes || 0)
        case 'downloads': return (b.downloads || 0) - (a.downloads || 0)
        default: return 0
      }
    })

  const categories = Array.from(new Set(DEMO_IMAGES.map(img => img.category).filter(Boolean)))

  const handleImageSelect = (id: string) => {
    const newSelected = new Set(selectedImages)
    if (newSelected.has(id)) {
      newSelected.delete(id)
    } else {
      newSelected.add(id)
    }
    setSelectedImages(newSelected)
  }

  const getImageOpacity = (imageId: string) => {
    if (!hoveredId) return 1
    if (imageId === hoveredId) return 1
    
    const hoveredIndex = filteredImages.findIndex(img => img.id === hoveredId)
    const currentIndex = filteredImages.findIndex(img => img.id === imageId)
    const distance = Math.abs(hoveredIndex - currentIndex)
    
    if (distance === 0) return 1
    if (distance === 1) return 0.6
    if (distance === 2) return 0.3
    return 0.1
  }

  const getImageBlur = (imageId: string) => {
    if (!hoveredId || imageId === hoveredId) return 0
    
    const hoveredIndex = filteredImages.findIndex(img => img.id === hoveredId)
    const currentIndex = filteredImages.findIndex(img => img.id === imageId)
    const distance = Math.abs(hoveredIndex - currentIndex)
    
    return Math.min(distance * 2, 8)
  }

  const getImageScale = (imageId: string) => {
    if (!hoveredId) return 1
    if (imageId === hoveredId) return 1.08
    return 0.95
  }

  const currentFullscreenImage = fullscreenImage ? DEMO_IMAGES.find(img => img.id === fullscreenImage) : null

  return (
    <div className="min-h-screen bg-gradient-to-br from-gray-900 via-gray-800 to-black">
      {/* Header */}
      <div className="sticky top-0 z-40 bg-gray-900/80 backdrop-blur-xl border-b border-gray-700/50">
        <div className="max-w-7xl mx-auto p-6">
          <motion.div 
            initial={{ opacity: 0, y: -20 }}
            animate={{ opacity: 1, y: 0 }}
            className="flex items-center justify-between"
          >
            <div className="flex items-center gap-4">
              <motion.div
                whileHover={{ scale: 1.1, rotate: 180 }}
                transition={{ type: "spring", stiffness: 400, damping: 10 }}
                className="w-12 h-12 bg-gradient-to-r from-purple-500 via-pink-500 to-orange-500 rounded-full flex items-center justify-center"
              >
                <Zap className="w-6 h-6 text-white" />
              </motion.div>
              <div>
                <h1 className="text-4xl font-bold text-white">Studio shots</h1>
                <p className="text-gray-400">{filteredImages.length} of {DEMO_IMAGES.length} tiles</p>
              </div>
            </div>
            
            <div className="flex items-center gap-3">
              <Badge 
                variant="secondary" 
                className="bg-green-500/20 text-green-400 border-green-500/50 px-4 py-2"
              >
                {isPublished ? 'Published' : 'Draft'}
              </Badge>
              
              <div className="flex items-center gap-2 bg-gray-800/50 rounded-lg p-2">
                <Button
                  variant={viewMode === 'grid' ? 'default' : 'ghost'}
                  size="sm"
                  onClick={() => setViewMode('grid')}
                  className="px-3"
                >
                  <Grid3x3 className="w-4 h-4" />
                </Button>
                <Button
                  variant={viewMode === 'masonry' ? 'default' : 'ghost'}
                  size="sm"
                  onClick={() => setViewMode('masonry')}
                  className="px-3"
                >
                  <Star className="w-4 h-4" />
                </Button>
              </div>
            </div>
          </motion.div>

          {/* Search and Filter Bar */}
          <motion.div 
            className="mt-6 flex items-center gap-4 flex-wrap"
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: 0.1 }}
          >
            <div className="relative flex-1 min-w-64">
              <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 w-4 h-4 text-gray-400" />
              <input
                type="text"
                placeholder="Search images by tags or description..."
                className="w-full pl-10 pr-4 py-2 bg-gray-800/50 border border-gray-700 rounded-lg text-white placeholder-gray-400 focus:border-blue-500 focus:outline-none transition-colors"
                value={searchTerm}
                onChange={(e) => setSearchTerm(e.target.value)}
              />
            </div>
            
            <select 
              className="px-4 py-2 bg-gray-800/50 border border-gray-700 rounded-lg text-white focus:border-blue-500 focus:outline-none"
              value={selectedCategory || ''}
              onChange={(e) => setSelectedCategory(e.target.value || null)}
            >
              <option value="">All categories</option>
              {categories.map(category => (
                <option key={category} value={category}>{category}</option>
              ))}
            </select>
            
            <select 
              className="px-4 py-2 bg-gray-800/50 border border-gray-700 rounded-lg text-white focus:border-blue-500 focus:outline-none"
              value={sortBy}
              onChange={(e) => setSortBy(e.target.value as 'recent' | 'likes' | 'downloads')}
            >
              <option value="recent">Recent</option>
              <option value="likes">Most liked</option>
              <option value="downloads">Most downloaded</option>
            </select>
          </motion.div>
        </div>
      </div>

      {/* Gallery Grid */}
      <div className="p-6">
        <div className="max-w-7xl mx-auto">
          <motion.div
            ref={containerRef}
            className="relative"
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            transition={{ duration: 0.5, delay: 0.2 }}
          >
            {/* Floating cursor indicator */}
            <AnimatePresence>
              {hoveredId && (
                <motion.div
                  className="fixed w-24 h-24 bg-white/10 backdrop-blur-md rounded-full border border-white/20 flex items-center justify-center pointer-events-none z-50"
                  style={{ x, y }}
                  initial={{ opacity: 0, scale: 0 }}
                  animate={{ opacity: 1, scale: 1 }}
                  exit={{ opacity: 0, scale: 0 }}
                  transition={{ type: "spring", stiffness: 400, damping: 20 }}
                >
                  <ArrowUpRight className="w-6 h-6 text-white" />
                </motion.div>
              )}
            </AnimatePresence>

            <div className="grid grid-cols-1 sm:grid-cols-2 md:grid-cols-3 lg:grid-cols-4 xl:grid-cols-5 gap-4">
              {filteredImages.map((image, index) => (
                <motion.div
                  key={image.id}
                  className="relative group cursor-pointer"
                  onHoverStart={() => setHoveredId(image.id)}
                  onHoverEnd={() => setHoveredId(null)}
                  initial={{ opacity: 0, y: 20 }}
                  animate={{ 
                    opacity: getImageOpacity(image.id),
                    y: 0,
                    scale: getImageScale(image.id)
                  }}
                  transition={{ 
                    duration: 0.6, 
                    delay: index * 0.03,
                    ease: [0.25, 0.25, 0, 1]
                  }}
                  style={{
                    filter: `blur(${getImageBlur(image.id)}px)`
                  }}
                  onClick={() => setFullscreenImage(image.id)}
                >
                  <Card className="overflow-hidden bg-gray-800/50 border-gray-700/50 hover:border-gray-600/80 transition-all duration-300">
                    <div className="aspect-square relative">
                      <img
                        src={image.src}
                        alt={image.alt}
                        className="w-full h-full object-cover transition-transform duration-500 group-hover:scale-110"
                      />
                      
                      {/* Gradient overlay */}
                      <div className="absolute inset-0 bg-gradient-to-t from-black/60 via-transparent to-transparent opacity-0 group-hover:opacity-100 transition-opacity duration-300" />
                      
                      {/* Selection indicator */}
                      <motion.div
                        className="absolute top-2 left-2"
                        initial={{ scale: 0 }}
                        animate={{ scale: selectedImages.has(image.id) ? 1 : 0 }}
                        transition={{ type: "spring", stiffness: 400, damping: 20 }}
                      >
                        <div className="w-6 h-6 bg-blue-500 rounded-full flex items-center justify-center">
                          <div className="w-2 h-2 bg-white rounded-full" />
                        </div>
                      </motion.div>

                      {/* Stats */}
                      <div className="absolute top-2 right-2 opacity-0 group-hover:opacity-100 transition-opacity duration-300">
                        <div className="flex flex-col gap-1">
                          <Badge className="bg-black/70 text-white border-none text-xs">
                            {image.category}
                          </Badge>
                          <div className="flex gap-1">
                            <Badge className="bg-black/70 text-white border-none text-xs">
                              ❤️ {image.likes}
                            </Badge>
                            <Badge className="bg-black/70 text-white border-none text-xs">
                              ⬇️ {image.downloads}
                            </Badge>
                          </div>
                        </div>
                      </div>

                      {/* Action buttons */}
                      <motion.div 
                        className="absolute bottom-3 right-3 flex gap-2 opacity-0 group-hover:opacity-100"
                        initial={{ y: 10 }}
                        whileHover={{ y: 0 }}
                        transition={{ duration: 0.2 }}
                      >
                        <motion.button
                          whileHover={{ scale: 1.1 }}
                          whileTap={{ scale: 0.9 }}
                          className="w-8 h-8 bg-white/20 backdrop-blur-sm rounded-full flex items-center justify-center hover:bg-white/30 transition-colors"
                          onClick={(e) => {
                            e.stopPropagation()
                            handleImageSelect(image.id)
                          }}
                        >
                          <Heart className={`w-4 h-4 ${selectedImages.has(image.id) ? 'text-red-400 fill-red-400' : 'text-white'}`} />
                        </motion.button>
                        
                        <motion.button
                          whileHover={{ scale: 1.1 }}
                          whileTap={{ scale: 0.9 }}
                          className="w-8 h-8 bg-white/20 backdrop-blur-sm rounded-full flex items-center justify-center hover:bg-white/30 transition-colors"
                        >
                          <Maximize2 className="w-4 h-4 text-white" />
                        </motion.button>
                      </motion.div>

                      {/* Hover overlay with enhanced effects */}
                      <AnimatePresence>
                        {hoveredId === image.id && (
                          <motion.div
                            className="absolute inset-0 bg-gradient-to-t from-blue-600/20 via-purple-600/10 to-transparent"
                            initial={{ opacity: 0 }}
                            animate={{ opacity: 1 }}
                            exit={{ opacity: 0 }}
                            transition={{ duration: 0.3 }}
                          />
                        )}
                      </AnimatePresence>
                    </div>
                  </Card>

                  {/* Enhanced hover glow effect */}
                  <AnimatePresence>
                    {hoveredId === image.id && (
                      <motion.div
                        className="absolute -inset-1 bg-gradient-to-r from-blue-500/20 via-purple-500/20 to-pink-500/20 rounded-lg blur-sm -z-10"
                        initial={{ opacity: 0, scale: 0.8 }}
                        animate={{ opacity: 1, scale: 1 }}
                        exit={{ opacity: 0, scale: 0.8 }}
                        transition={{ duration: 0.3 }}
                      />
                    )}
                  </AnimatePresence>
                </motion.div>
              ))}
            </div>
          </motion.div>

          {/* Footer Actions */}
          <motion.div 
            className="flex items-center justify-between mt-8 pt-6 border-t border-gray-700/50"
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ delay: 0.8 }}
          >
            <div className="flex items-center gap-4">
              <p className="text-gray-400">
                {selectedImages.size > 0 && `${selectedImages.size} selected • `}
                Last updated 2 hours ago
              </p>
            </div>
            
            <div className="flex items-center gap-3">
              <AnimatePresence>
                {selectedImages.size > 0 && (
                  <motion.div
                    initial={{ opacity: 0, x: 20 }}
                    animate={{ opacity: 1, x: 0 }}
                    exit={{ opacity: 0, x: 20 }}
                    className="flex gap-2"
                  >
                    <Button variant="outline" size="sm">
                      Export Selected
                    </Button>
                    <Button variant="outline" size="sm">
                      Create Collection
                    </Button>
                  </motion.div>
                )}
              </AnimatePresence>
              
              <Button className="bg-blue-600 hover:bg-blue-700">
                Go to collection
              </Button>
              <Button variant="outline">
                Edit rules
              </Button>
            </div>
          </motion.div>
        </div>
      </div>

      {/* Fullscreen Modal */}
      <AnimatePresence>
        {fullscreenImage && currentFullscreenImage && (
          <motion.div
            className="fixed inset-0 z-50 bg-black/95 backdrop-blur-sm flex items-center justify-center"
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
            onClick={() => setFullscreenImage(null)}
          >
            <motion.div
              className="relative max-w-6xl max-h-[90vh] w-full h-full flex items-center justify-center p-6"
              initial={{ scale: 0.9, opacity: 0 }}
              animate={{ scale: 1, opacity: 1 }}
              exit={{ scale: 0.9, opacity: 0 }}
              transition={{ type: "spring", stiffness: 300, damping: 30 }}
              onClick={(e) => e.stopPropagation()}
            >
              <img
                src={currentFullscreenImage.src}
                alt={currentFullscreenImage.alt}
                className="max-w-full max-h-full object-contain rounded-lg"
              />
              
              {/* Close button */}
              <button
                className="absolute top-4 right-4 w-10 h-10 bg-white/20 backdrop-blur-sm rounded-full flex items-center justify-center hover:bg-white/30 transition-colors"
                onClick={() => setFullscreenImage(null)}
              >
                <X className="w-5 h-5 text-white" />
              </button>
              
              {/* Navigation buttons */}
              <button
                className="absolute left-4 top-1/2 transform -translate-y-1/2 w-12 h-12 bg-white/20 backdrop-blur-sm rounded-full flex items-center justify-center hover:bg-white/30 transition-colors"
                onClick={() => {
                  const currentIndex = DEMO_IMAGES.findIndex(img => img.id === fullscreenImage)
                  const prevIndex = (currentIndex - 1 + DEMO_IMAGES.length) % DEMO_IMAGES.length
                  setFullscreenImage(DEMO_IMAGES[prevIndex].id)
                }}
              >
                <ChevronLeft className="w-6 h-6 text-white" />
              </button>
              
              <button
                className="absolute right-4 top-1/2 transform -translate-y-1/2 w-12 h-12 bg-white/20 backdrop-blur-sm rounded-full flex items-center justify-center hover:bg-white/30 transition-colors"
                onClick={() => {
                  const currentIndex = DEMO_IMAGES.findIndex(img => img.id === fullscreenImage)
                  const nextIndex = (currentIndex + 1) % DEMO_IMAGES.length
                  setFullscreenImage(DEMO_IMAGES[nextIndex].id)
                }}
              >
                <ChevronRight className="w-6 h-6 text-white" />
              </button>
              
              {/* Image info */}
              <div className="absolute bottom-4 left-4 right-4 bg-black/50 backdrop-blur-sm rounded-lg p-4">
                <h3 className="text-white text-lg font-semibold mb-2">{currentFullscreenImage.alt}</h3>
                <div className="flex items-center justify-between">
                  <div className="flex items-center gap-4">
                    <Badge className="bg-white/20 text-white border-none">
                      {currentFullscreenImage.category}
                    </Badge>
                    <span className="text-gray-300 text-sm">❤️ {currentFullscreenImage.likes}</span>
                    <span className="text-gray-300 text-sm">⬇️ {currentFullscreenImage.downloads}</span>
                  </div>
                  <div className="flex gap-2">
                    <Button size="sm" variant="outline">
                      <Download className="w-4 h-4 mr-2" />
                      Download
                    </Button>
                    <Button size="sm" variant="outline">
                      <Share2 className="w-4 h-4 mr-2" />
                      Share
                    </Button>
                  </div>
                </div>
                {currentFullscreenImage.tags && (
                  <div className="flex gap-1 mt-2">
                    {currentFullscreenImage.tags.map(tag => (
                      <Badge key={tag} className="bg-white/10 text-white border-none text-xs">
                        #{tag}
                      </Badge>
                    ))}
                  </div>
                )}
              </div>
            </motion.div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  )
} 