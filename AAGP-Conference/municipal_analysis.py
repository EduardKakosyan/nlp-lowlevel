# /// script
# requires-python = ">=3.13"
# dependencies = [
#     "geopandas>=0.14.0",
#     "pandas>=2.0.0",
#     "matplotlib>=3.7.0",
#     "seaborn>=0.12.0",
#     "folium>=0.15.0",
#     "contextily>=1.4.0",
#     "openai>=1.0.0",
#     "reportlab>=4.0.0",
#     "requests>=2.31.0",
#     "numpy>=1.24.0",
#     "scipy>=1.11.0",
#     "shapely>=2.0.0",
#     "fiona>=1.9.0",
#     "pyproj>=3.6.0"
# ]
# ///

"""
AAGP Workshop: Generative AI for Municipal Planning
Automated analysis of urban population density changes over time
"""

import json
import logging
import os
import warnings
from pathlib import Path
from typing import Dict, List, Tuple

import contextily as ctx
import folium
import geopandas as gpd
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
from openai import OpenAI
from reportlab.lib import colors
from reportlab.lib.pagesizes import letter
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Image
from shapely.geometry import Point, Polygon

# Suppress warnings for cleaner output
warnings.filterwarnings('ignore')

# Configure logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


class MunicipalAnalyzer:
    """Main class for municipal population density analysis"""

    def __init__(self, config_path: str = 'api_config.json'):
        """Initialize the analyzer with configuration"""
        self.config = self._load_config(config_path)
        self.client = None
        if self.config.get('api_key'):
            self.client = OpenAI(api_key=self.config['api_key'])

        # Data storage
        self.boundaries_2016 = None
        self.boundaries_2021 = None
        self.population_2016 = None
        self.population_2021 = None
        self.combined_data = None
        self.analysis_results = {}

    def _load_config(self, config_path: str) -> Dict:
        """Load API configuration"""
        try:
            with open(config_path, 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            logger.warning(f'Config file {config_path} not found. Using defaults.')
            return {
                'api_key': 'mock-key',
                'model_name': 'gpt-3.5-turbo',
                'max_tokens': 1000,
                'temperature': 0.7,
            }

    def create_mock_data(self):
        """Create mock municipal data for demonstration"""
        logger.info('Creating mock municipal data...')

        # Mock municipal boundaries for Atlantic Canada
        municipalities = [
            {
                'MUNI_ID': 'NS001',
                'MUNI_NAME': 'Halifax',
                'PROVINCE': 'NS',
                'lat': 44.6488,
                'lon': -63.5752,
            },
            {
                'MUNI_ID': 'NS002',
                'MUNI_NAME': 'Sydney',
                'PROVINCE': 'NS',
                'lat': 46.1351,
                'lon': -60.1831,
            },
            {
                'MUNI_ID': 'NB001',
                'MUNI_NAME': 'Saint John',
                'PROVINCE': 'NB',
                'lat': 45.2733,
                'lon': -66.0633,
            },
            {
                'MUNI_ID': 'NB002',
                'MUNI_NAME': 'Moncton',
                'PROVINCE': 'NB',
                'lat': 46.0878,
                'lon': -64.7782,
            },
            {
                'MUNI_ID': 'PE001',
                'MUNI_NAME': 'Charlottetown',
                'PROVINCE': 'PE',
                'lat': 46.2382,
                'lon': -63.1311,
            },
            {
                'MUNI_ID': 'NL001',
                'MUNI_NAME': "St. John's",
                'PROVINCE': 'NL',
                'lat': 47.5615,
                'lon': -52.7126,
            },
        ]

        # Create mock boundaries (simplified squares around city centers)
        def create_boundary_polygon(lat, lon, size=0.1):
            """Create a simple square polygon around a point"""
            return Polygon(
                [
                    (lon - size, lat - size),
                    (lon + size, lat - size),
                    (lon + size, lat + size),
                    (lon - size, lat + size),
                    (lon - size, lat - size),
                ]
            )

        # Create 2016 boundaries
        boundaries_2016_data = []
        for muni in municipalities:
            boundaries_2016_data.append(
                {
                    'MUNI_ID': muni['MUNI_ID'],
                    'MUNI_NAME': muni['MUNI_NAME'],
                    'PROVINCE': muni['PROVINCE'],
                    'geometry': create_boundary_polygon(muni['lat'], muni['lon']),
                }
            )

        self.boundaries_2016 = gpd.GeoDataFrame(boundaries_2016_data, crs='EPSG:4326')

        # Create 2021 boundaries (slightly expanded)
        boundaries_2021_data = []
        for muni in municipalities:
            boundaries_2021_data.append(
                {
                    'MUNI_ID': muni['MUNI_ID'],
                    'MUNI_NAME': muni['MUNI_NAME'],
                    'PROVINCE': muni['PROVINCE'],
                    'geometry': create_boundary_polygon(
                        muni['lat'], muni['lon'], size=0.11
                    ),  # Slightly larger
                }
            )

        self.boundaries_2021 = gpd.GeoDataFrame(boundaries_2021_data, crs='EPSG:4326')

        # Create mock population data
        np.random.seed(42)  # For reproducible results

        population_2016_data = []
        population_2021_data = []

        base_populations = {
            'NS001': 403131,  # Halifax
            'NS002': 94285,  # Sydney
            'NB001': 67575,  # Saint John
            'NB002': 71889,  # Moncton
            'PE001': 36094,  # Charlottetown
            'NL001': 108860,  # St. John's
        }

        for muni_id, base_pop in base_populations.items():
            # 2016 data
            area_km2 = np.random.uniform(50, 200)  # Random area between 50-200 km²
            households_2016 = int(base_pop / 2.3)  # Average household size

            population_2016_data.append(
                {
                    'MUNI_ID': muni_id,
                    'POPULATION': base_pop,
                    'HOUSEHOLDS': households_2016,
                    'LAND_AREA_KM2': area_km2,
                }
            )

            # 2021 data (with growth)
            growth_rate = np.random.uniform(0.05, 0.15)  # 5-15% growth
            pop_2021 = int(base_pop * (1 + growth_rate))
            households_2021 = int(pop_2021 / 2.2)  # Slightly smaller household size

            population_2021_data.append(
                {
                    'MUNI_ID': muni_id,
                    'POPULATION': pop_2021,
                    'HOUSEHOLDS': households_2021,
                    'LAND_AREA_KM2': area_km2,  # Same area
                }
            )

        self.population_2016 = pd.DataFrame(population_2016_data)
        self.population_2021 = pd.DataFrame(population_2021_data)

        logger.info('Mock data created successfully')

    def load_and_validate_data(self):
        """Load and validate all input data"""
        logger.info('Loading and validating data...')

        # For this demo, we'll use the mock data
        if self.boundaries_2016 is None:
            self.create_mock_data()

        # Validate data integrity
        assert len(self.boundaries_2016) == len(self.boundaries_2021), 'Boundary count mismatch'
        assert len(self.population_2016) == len(self.population_2021), (
            'Population data count mismatch'
        )

        # Check for matching MUNI_IDs
        boundary_ids_2016 = set(self.boundaries_2016['MUNI_ID'])
        boundary_ids_2021 = set(self.boundaries_2021['MUNI_ID'])
        pop_ids_2016 = set(self.population_2016['MUNI_ID'])
        pop_ids_2021 = set(self.population_2021['MUNI_ID'])

        assert boundary_ids_2016 == boundary_ids_2021, 'Boundary MUNI_ID mismatch between years'
        assert pop_ids_2016 == pop_ids_2021, 'Population MUNI_ID mismatch between years'
        assert boundary_ids_2016 == pop_ids_2016, (
            'MUNI_ID mismatch between boundaries and population'
        )

        logger.info('Data validation completed successfully')

    def prepare_geospatial_data(self):
        """Standardize CRS and perform spatial joins"""
        logger.info('Preparing geospatial data...')

        # Convert to Statistics Canada Lambert projection (EPSG:3347)
        target_crs = 'EPSG:3347'
        self.boundaries_2016 = self.boundaries_2016.to_crs(target_crs)
        self.boundaries_2021 = self.boundaries_2021.to_crs(target_crs)

        # Perform spatial joins
        combined_2016 = self.boundaries_2016.merge(self.population_2016, on='MUNI_ID')
        combined_2021 = self.boundaries_2021.merge(self.population_2021, on='MUNI_ID')

        # Store combined data
        self.combined_data = {'2016': combined_2016, '2021': combined_2021}

        logger.info('Geospatial data preparation completed')

    def calculate_population_density(self):
        """Calculate population density for both time periods"""
        logger.info('Calculating population density...')

        for year in ['2016', '2021']:
            data = self.combined_data[year]

            # Calculate area in km² (convert from m²)
            data['CALCULATED_AREA_KM2'] = data.geometry.area / 1_000_000

            # Use provided area if available, otherwise use calculated
            data['AREA_KM2'] = data.get('LAND_AREA_KM2', data['CALCULATED_AREA_KM2'])

            # Calculate density
            data['DENSITY_PER_KM2'] = data['POPULATION'] / data['AREA_KM2']

            # Classify density
            data['DENSITY_CLASS'] = pd.cut(
                data['DENSITY_PER_KM2'],
                bins=[0, 50, 150, 500, float('inf')],
                labels=['Low', 'Medium', 'High', 'Very High'],
            )

            self.combined_data[year] = data

        logger.info('Population density calculation completed')

    def analyze_changes(self):
        """Analyze changes between time periods"""
        logger.info('Analyzing population density changes...')

        data_2016 = self.combined_data['2016']
        data_2021 = self.combined_data['2021']

        # Merge data for comparison
        comparison = data_2016[['MUNI_ID', 'MUNI_NAME', 'DENSITY_PER_KM2']].merge(
            data_2021[['MUNI_ID', 'DENSITY_PER_KM2']], on='MUNI_ID', suffixes=('_2016', '_2021')
        )

        # Calculate changes
        comparison['DENSITY_CHANGE_ABS'] = (
            comparison['DENSITY_PER_KM2_2021'] - comparison['DENSITY_PER_KM2_2016']
        )
        comparison['DENSITY_CHANGE_PCT'] = (
            comparison['DENSITY_CHANGE_ABS'] / comparison['DENSITY_PER_KM2_2016']
        ) * 100

        # Classify changes
        comparison['CHANGE_CLASS'] = pd.cut(
            comparison['DENSITY_CHANGE_PCT'],
            bins=[-float('inf'), -5, 5, 15, float('inf')],
            labels=['Decline', 'Stable', 'Moderate Growth', 'High Growth'],
        )

        # Generate statistics
        stats = {
            'mean_change_pct': comparison['DENSITY_CHANGE_PCT'].mean(),
            'median_change_pct': comparison['DENSITY_CHANGE_PCT'].median(),
            'std_change_pct': comparison['DENSITY_CHANGE_PCT'].std(),
            'growth_municipalities': len(comparison[comparison['DENSITY_CHANGE_PCT'] > 5]),
            'decline_municipalities': len(comparison[comparison['DENSITY_CHANGE_PCT'] < -5]),
            'stable_municipalities': len(comparison[abs(comparison['DENSITY_CHANGE_PCT']) <= 5]),
        }

        self.analysis_results = {'comparison': comparison, 'statistics': stats}

        logger.info('Change analysis completed')

    def create_visualizations(self):
        """Create choropleth maps and visualizations"""
        logger.info('Creating visualizations...')

        # Set up the plotting style
        plt.style.use('seaborn-v0_8')

        # Create figure with subplots
        fig, axes = plt.subplots(1, 3, figsize=(20, 6))

        # 2016 Density Map
        self.combined_data['2016'].plot(
            column='DENSITY_PER_KM2',
            cmap='YlOrRd',
            legend=True,
            ax=axes[0],
            legend_kwds={'label': 'Population Density (people/km²)'},
        )
        axes[0].set_title('Population Density 2016', fontsize=14, fontweight='bold')
        axes[0].axis('off')

        # 2021 Density Map
        self.combined_data['2021'].plot(
            column='DENSITY_PER_KM2',
            cmap='YlOrRd',
            legend=True,
            ax=axes[1],
            legend_kwds={'label': 'Population Density (people/km²)'},
        )
        axes[1].set_title('Population Density 2021', fontsize=14, fontweight='bold')
        axes[1].axis('off')

        # Change Map
        comparison_gdf = self.combined_data['2021'].merge(
            self.analysis_results['comparison'][['MUNI_ID', 'DENSITY_CHANGE_PCT']], on='MUNI_ID'
        )

        comparison_gdf.plot(
            column='DENSITY_CHANGE_PCT',
            cmap='RdYlGn',
            legend=True,
            ax=axes[2],
            legend_kwds={'label': 'Density Change (%)'},
        )
        axes[2].set_title('Population Density Change 2016-2021', fontsize=14, fontweight='bold')
        axes[2].axis('off')

        plt.tight_layout()
        plt.savefig('density_maps.png', dpi=300, bbox_inches='tight')
        plt.close()

        # Create summary statistics plot
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))

        # Density change distribution
        self.analysis_results['comparison']['DENSITY_CHANGE_PCT'].hist(
            bins=10, ax=ax1, color='skyblue', alpha=0.7, edgecolor='black'
        )
        ax1.set_xlabel('Density Change (%)')
        ax1.set_ylabel('Number of Municipalities')
        ax1.set_title('Distribution of Density Changes')
        ax1.axvline(0, color='red', linestyle='--', alpha=0.7)

        # Change classification pie chart
        change_counts = self.analysis_results['comparison']['CHANGE_CLASS'].value_counts()
        ax2.pie(change_counts.values, labels=change_counts.index, autopct='%1.1f%%', startangle=90)
        ax2.set_title('Municipality Change Classification')

        plt.tight_layout()
        plt.savefig('analysis_summary.png', dpi=300, bbox_inches='tight')
        plt.close()

        logger.info('Visualizations created and saved')

    def create_interactive_map(self):
        """Create interactive Folium map"""
        logger.info('Creating interactive map...')

        # Convert back to WGS84 for Folium
        data_2021_wgs84 = self.combined_data['2021'].to_crs('EPSG:4326')
        comparison_data = data_2021_wgs84.merge(
            self.analysis_results['comparison'][
                ['MUNI_ID', 'DENSITY_CHANGE_PCT', 'DENSITY_CHANGE_ABS']
            ],
            on='MUNI_ID',
        )

        # Calculate map center
        bounds = comparison_data.total_bounds
        center_lat = (bounds[1] + bounds[3]) / 2
        center_lon = (bounds[0] + bounds[2]) / 2

        # Create map
        m = folium.Map(location=[center_lat, center_lon], zoom_start=6, tiles='OpenStreetMap')

        # Add choropleth layer
        folium.Choropleth(
            geo_data=comparison_data,
            data=comparison_data,
            columns=['MUNI_ID', 'DENSITY_CHANGE_PCT'],
            key_on='feature.properties.MUNI_ID',
            fill_color='RdYlGn',
            fill_opacity=0.7,
            line_opacity=0.2,
            legend_name='Density Change (%)',
        ).add_to(m)

        # Add municipality labels and popups
        for idx, row in comparison_data.iterrows():
            folium.Marker(
                location=[row.geometry.centroid.y, row.geometry.centroid.x],
                popup=folium.Popup(
                    f"""
                    <b>{row['MUNI_NAME']}</b><br>
                    Province: {row['PROVINCE']}<br>
                    Density Change: {row['DENSITY_CHANGE_PCT']:.1f}%<br>
                    Absolute Change: {row['DENSITY_CHANGE_ABS']:.1f} people/km²
                    """,
                    max_width=300,
                ),
                tooltip=row['MUNI_NAME'],
            ).add_to(m)

        # Save map
        m.save('interactive_map.html')
        logger.info('Interactive map created and saved')

    def generate_ai_insights(self):
        """Generate AI-powered insights about the analysis"""
        logger.info('Generating AI insights...')

        if not self.client:
            # Mock AI response for demonstration
            insights = [
                'Halifax shows the strongest population density growth at 9.1%, indicating robust economic development and urban vitality.',
                'Maritime provinces demonstrate consistent moderate growth patterns, suggesting sustainable regional development.',
                'Infrastructure capacity assessments are recommended for municipalities showing >10% density increases.',
                'The balanced growth across Atlantic Canada indicates positive inter-provincial migration patterns.',
                'Municipal planning should focus on sustainable development practices to maintain growth momentum.',
            ]
            logger.info('Using mock AI insights (no API key provided)')
            return insights

        # Prepare data summary for AI
        stats = self.analysis_results['statistics']
        comparison = self.analysis_results['comparison']

        data_summary = f"""
        Municipal Population Density Analysis (2016-2021):
        
        Overall Statistics:
        - Average density change: {stats['mean_change_pct']:.1f}%
        - Median density change: {stats['median_change_pct']:.1f}%
        - Standard deviation: {stats['std_change_pct']:.1f}%
        - Growing municipalities: {stats['growth_municipalities']}
        - Declining municipalities: {stats['decline_municipalities']}
        - Stable municipalities: {stats['stable_municipalities']}
        
        Municipality Details:
        {comparison[['MUNI_NAME', 'DENSITY_CHANGE_PCT']].to_string(index=False)}
        """

        try:
            response = self.client.chat.completions.create(
                model=self.config['model_name'],
                messages=[
                    {
                        'role': 'system',
                        'content': 'You are an expert urban planner analyzing population density changes. Provide 3-5 actionable insights and recommendations for municipal planning based on the data.',
                    },
                    {
                        'role': 'user',
                        'content': f'Analyze this municipal population density data and provide planning insights:\n\n{data_summary}',
                    },
                ],
                max_tokens=self.config['max_tokens'],
                temperature=self.config['temperature'],
            )

            insights_text = response.choices[0].message.content
            insights = [
                insight.strip()
                for insight in insights_text.split('\n')
                if insight.strip() and not insight.strip().startswith('#')
            ]

        except Exception as e:
            logger.error(f'AI API error: {e}')
            # Fallback to mock insights
            insights = [
                'Population density analysis indicates healthy regional growth patterns.',
                'Infrastructure planning should prioritize municipalities with >10% growth rates.',
                'Sustainable development practices are essential for continued growth.',
                'Inter-municipal coordination recommended for balanced regional development.',
                'Regular monitoring of density changes will inform future planning decisions.',
            ]

        self.analysis_results['ai_insights'] = insights
        logger.info('AI insights generated')
        return insights

    def generate_report(self):
        """Generate automated planning report"""
        logger.info('Generating automated report...')

        # Create PDF report
        doc = SimpleDocTemplate('municipal_planning_report.pdf', pagesize=letter)

        styles = getSampleStyleSheet()
        story = []

        # Title
        title = Paragraph('Municipal Population Density Analysis Report', styles['Title'])
        story.append(title)
        story.append(Spacer(1, 12))

        # Executive Summary
        summary_title = Paragraph('Executive Summary', styles['Heading1'])
        story.append(summary_title)

        stats = self.analysis_results['statistics']
        summary_text = f"""
        This report analyzes population density changes across Atlantic Canadian municipalities 
        between 2016 and 2021. Key findings include an average density increase of 
        {stats['mean_change_pct']:.1f}%, with {stats['growth_municipalities']} municipalities 
        showing significant growth. The analysis reveals healthy regional development patterns 
        that require strategic infrastructure planning.
        """

        summary_para = Paragraph(summary_text, styles['Normal'])
        story.append(summary_para)
        story.append(Spacer(1, 12))

        # Key Findings
        findings_title = Paragraph('Key Findings', styles['Heading1'])
        story.append(findings_title)

        comparison = self.analysis_results['comparison']
        top_growth = comparison.nlargest(3, 'DENSITY_CHANGE_PCT')

        findings_text = f"""
        <b>Top Growing Municipalities:</b><br/>
        """

        for _, row in top_growth.iterrows():
            findings_text += f'• {row["MUNI_NAME"]}: +{row["DENSITY_CHANGE_PCT"]:.1f}%<br/>'

        findings_text += f"""
        <br/><b>Regional Statistics:</b><br/>
        • Average growth rate: {stats['mean_change_pct']:.1f}%<br/>
        • Municipalities with positive growth: {stats['growth_municipalities']}<br/>
        • Stable municipalities: {stats['stable_municipalities']}<br/>
        """

        findings_para = Paragraph(findings_text, styles['Normal'])
        story.append(findings_para)
        story.append(Spacer(1, 12))

        # AI Insights
        insights_title = Paragraph('Planning Recommendations', styles['Heading1'])
        story.append(insights_title)

        if 'ai_insights' in self.analysis_results:
            for i, insight in enumerate(self.analysis_results['ai_insights'][:5], 1):
                insight_para = Paragraph(f'{i}. {insight}', styles['Normal'])
                story.append(insight_para)
                story.append(Spacer(1, 6))

        # Add visualizations if they exist
        if os.path.exists('density_maps.png'):
            story.append(Spacer(1, 12))
            maps_title = Paragraph('Density Analysis Maps', styles['Heading1'])
            story.append(maps_title)

            # Add image
            img = Image('density_maps.png', width=500, height=150)
            story.append(img)

        # Build PDF
        doc.build(story)

        logger.info('Report generated successfully')

    def run_complete_analysis(self):
        """Run the complete analysis workflow"""
        logger.info('Starting complete municipal analysis workflow...')

        try:
            # Step 1: Load and validate data
            self.load_and_validate_data()

            # Step 2: Prepare geospatial data
            self.prepare_geospatial_data()

            # Step 3: Calculate population density
            self.calculate_population_density()

            # Step 4: Analyze changes
            self.analyze_changes()

            # Step 5: Create visualizations
            self.create_visualizations()

            # Step 6: Create interactive map
            self.create_interactive_map()

            # Step 7: Generate AI insights
            self.generate_ai_insights()

            # Step 8: Generate report
            self.generate_report()

            logger.info('Analysis workflow completed successfully!')

            # Print summary
            self._print_summary()

        except Exception as e:
            logger.error(f'Analysis failed: {e}')
            raise

    def _print_summary(self):
        """Print analysis summary to console"""
        print('\n' + '=' * 60)
        print('MUNICIPAL POPULATION DENSITY ANALYSIS SUMMARY')
        print('=' * 60)

        stats = self.analysis_results['statistics']
        print(f'Average density change: {stats["mean_change_pct"]:.1f}%')
        print(f'Growing municipalities: {stats["growth_municipalities"]}')
        print(f'Stable municipalities: {stats["stable_municipalities"]}')
        print(f'Declining municipalities: {stats["decline_municipalities"]}')

        print('\nTop 3 Growing Municipalities:')
        comparison = self.analysis_results['comparison']
        top_growth = comparison.nlargest(3, 'DENSITY_CHANGE_PCT')

        for _, row in top_growth.iterrows():
            print(f'  {row["MUNI_NAME"]}: +{row["DENSITY_CHANGE_PCT"]:.1f}%')

        print('\nGenerated Files:')
        print('  • density_maps.png - Choropleth maps')
        print('  • analysis_summary.png - Statistical summaries')
        print('  • interactive_map.html - Interactive web map')
        print('  • municipal_planning_report.pdf - Complete report')

        if 'ai_insights' in self.analysis_results:
            print(
                f'\nAI Insights Generated: {len(self.analysis_results["ai_insights"])} recommendations'
            )

        print('=' * 60)


def main():
    """Main execution function"""
    print('AAGP Workshop: Generative AI for Municipal Planning')
    print('Starting automated population density analysis...')

    # Initialize analyzer
    analyzer = MunicipalAnalyzer()

    # Run complete analysis
    analyzer.run_complete_analysis()

    print('\nWorkshop demonstration completed successfully!')
    print('Check the generated files for detailed results.')


if __name__ == '__main__':
    main()
