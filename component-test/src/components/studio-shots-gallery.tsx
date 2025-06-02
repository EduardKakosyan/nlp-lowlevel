'use client'

import React, { useState, useRef, useEffect } from 'react'
import { motion, AnimatePresence, useMotionValue, useSpring } from 'framer-motion'
import { Card } from '@/components/ui/card'
import { Badge } from '@/components/ui/badge'
import { Button } from '@/components/ui/button'
import { Star, ArrowUpRight, Download, Heart, Share2 } from 'lucide-react'

interface ImageData {
  id: string
  src: string
  alt: string
  category?: string
}

const DEMO_IMAGES: ImageData[] = [
  {
    id: '1',
    src: 'https://images.unsplash.com/photo-1558618047-3c8c76ca7d13?w=400&h=300&fit=crop',
    alt: 'Person reaching up in cosmic scene',
    category: 'Portrait'
  },
  {
    id: '2', 
    src: 'https://images.unsplash.com/photo-1506905925346-21bda4d32df4?w=400&h=300&fit=crop',
    alt: 'Mountain landscape at golden hour',
    category: 'Landscape'
  },
  {
    id: '3',
    src: 'https://images.unsplash.com/photo-1558618047-3c8c76ca7d13?w=400&h=300&fit=crop',
    alt: 'Person reaching up in cosmic scene',
    category: 'Portrait'
  },
  {
    id: '4',
    src: 'https://images.unsplash.com/photo-1506905925346-21bda4d32df4?w=400&h=300&fit=crop',
    alt: 'Mountain landscape at golden hour',
    category: 'Landscape'
  },
  {
    id: '5',
    src: 'https://images.unsplash.com/photo-1519904981063-b0cf448d479e?w=400&h=300&fit=crop',
    alt: 'Mountain peaks in winter',
    category: 'Nature'
  },
  {
    id: '6',
    src: 'https://images.unsplash.com/photo-1511671782779-c97d3d27a1d4?w=400&h=300&fit=crop',
    alt: 'Recording studio microphone',
    category: 'Music'
  },
  {
    id: '7',
    src: 'https://images.unsplash.com/photo-1506905925346-21bda4d32df4?w=400&h=300&fit=crop',
    alt: 'Mountain landscape at golden hour',
    category: 'Landscape'
  },
  {
    id: '8',
    src: 'https://images.unsplash.com/photo-1551698618-1dfe5d97d256?w=400&h=300&fit=crop',
    alt: 'Portrait with blue tones',
    category: 'Portrait'
  },
  {
    id: '9',
    src: 'https://images.unsplash.com/photo-1511671782779-c97d3d27a1d4?w=400&h=300&fit=crop',
    alt: 'Recording studio microphone',
    category: 'Music'
  },
  {
    id: '10',
    src: 'https://images.unsplash.com/photo-1558618047-3c8c76ca7d13?w=400&h=300&fit=crop',
    alt: 'Person reaching up in cosmic scene',
    category: 'Portrait'
  }
]

export default function StudioShotsGallery() {
  const [hoveredId, setHoveredId] = useState<string | null>(null)
  const [selectedImages, setSelectedImages] = useState<Set<string>>(new Set())
  const [viewMode, setViewMode] = useState<'grid' | 'masonry'>('grid')
  const [isPublished, setIsPublished] = useState(true)
  const containerRef = useRef<HTMLDivElement>(null)
  
  const mouseX = useMotionValue(0)
  const mouseY = useMotionValue(0)
  const springConfig = { damping: 25, stiffness: 700 }
  const x = useSpring(mouseX, springConfig)
  const y = useSpring(mouseY, springConfig)

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
    
    // Calculate distance-based opacity for surrounding images
    const hoveredIndex = DEMO_IMAGES.findIndex(img => img.id === hoveredId)
    const currentIndex = DEMO_IMAGES.findIndex(img => img.id === imageId)
    const distance = Math.abs(hoveredIndex - currentIndex)
    
    // Create layered fade effect
    if (distance === 0) return 1
    if (distance === 1) return 0.7
    if (distance === 2) return 0.4
    return 0.15
  }

  const getImageBlur = (imageId: string) => {
    if (!hoveredId || imageId === hoveredId) return 0
    
    const hoveredIndex = DEMO_IMAGES.findIndex(img => img.id === hoveredId)
    const currentIndex = DEMO_IMAGES.findIndex(img => img.id === imageId)
    const distance = Math.abs(hoveredIndex - currentIndex)
    
    return Math.min(distance * 1.5, 6)
  }

  const getImageScale = (imageId: string) => {
    if (!hoveredId) return 1
    if (imageId === hoveredId) return 1.05
    return 0.98
  }

  return (
    <div className="min-h-screen bg-gradient-to-br from-gray-900 via-gray-800 to-black p-6">
      <div className="max-w-7xl mx-auto">
        {/* Header */}
        <motion.div 
          initial={{ opacity: 0, y: -20 }}
          animate={{ opacity: 1, y: 0 }}
          className="flex items-center justify-between mb-8"
        >
          <div className="flex items-center gap-4">
            <motion.div
              whileHover={{ scale: 1.1, rotate: 180 }}
              transition={{ type: "spring", stiffness: 400, damping: 10 }}
              className="w-12 h-12 bg-gradient-to-r from-purple-500 to-pink-500 rounded-full flex items-center justify-center"
            >
              <Star className="w-6 h-6 text-white" />
            </motion.div>
            <div>
              <h1 className="text-4xl font-bold text-white">Studio shots</h1>
              <p className="text-gray-400">{DEMO_IMAGES.length} tiles</p>
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
                1×1
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

        {/* Gallery Grid */}
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
                className="fixed w-20 h-20 bg-white/10 backdrop-blur-md rounded-full border border-white/20 flex items-center justify-center pointer-events-none z-50"
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

          <div className="grid grid-cols-5 gap-4">
            {DEMO_IMAGES.map((image, index) => (
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
                  delay: index * 0.05,
                  ease: [0.25, 0.25, 0, 1]
                }}
                style={{
                  filter: `blur(${getImageBlur(image.id)}px)`
                }}
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

                    {/* Category badge */}
                    {image.category && (
                      <Badge 
                        className="absolute top-2 right-2 bg-black/70 text-white border-none opacity-0 group-hover:opacity-100 transition-opacity duration-300"
                      >
                        {image.category}
                      </Badge>
                    )}

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
                        <Share2 className="w-4 h-4 text-white" />
                      </motion.button>
                      
                      <motion.button
                        whileHover={{ scale: 1.1 }}
                        whileTap={{ scale: 0.9 }}
                        className="w-8 h-8 bg-white/20 backdrop-blur-sm rounded-full flex items-center justify-center hover:bg-white/30 transition-colors"
                      >
                        <Download className="w-4 h-4 text-white" />
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
  )
} 