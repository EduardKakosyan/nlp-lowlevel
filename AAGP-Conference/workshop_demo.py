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
AAGP Workshop Demo Script
Simplified version for live demonstration with timing
"""

import time
from municipal_analysis import MunicipalAnalyzer


def workshop_demo():
    """Run the workshop demonstration with timing and explanations"""

    print('🏛️  AAGP Workshop: Generative AI for Municipal Planning')
    print('=' * 60)
    print('Live demonstration of automated population density analysis\n')

    # Initialize analyzer
    analyzer = MunicipalAnalyzer()

    # Phase 1: Setup (5 minutes)
    print('📊 PHASE 1: Setup and Data Loading (5 minutes)')
    print('-' * 50)
    start_time = time.time()

    print('• Loading Atlantic Canada municipal data...')
    analyzer.load_and_validate_data()
    print('• Validating data integrity and spatial consistency...')
    print('• Standardizing coordinate reference systems...')
    analyzer.prepare_geospatial_data()

    setup_time = time.time() - start_time
    print(f'✅ Setup completed in {setup_time:.1f} seconds\n')

    # Phase 2: Analysis (10 minutes)
    print('🔍 PHASE 2: Population Density Analysis (10 minutes)')
    print('-' * 50)
    start_time = time.time()

    print('• Calculating population densities for 2016 and 2021...')
    analyzer.calculate_population_density()
    print('• Analyzing density changes and growth patterns...')
    analyzer.analyze_changes()

    # Show some live results
    stats = analyzer.analysis_results['statistics']
    comparison = analyzer.analysis_results['comparison']

    print(f'• Average density change: {stats["mean_change_pct"]:.1f}%')
    print(f'• Growing municipalities: {stats["growth_municipalities"]}')
    print(
        f'• Top growth: {comparison.nlargest(1, "DENSITY_CHANGE_PCT").iloc[0]["MUNI_NAME"]} '
        f'(+{comparison.nlargest(1, "DENSITY_CHANGE_PCT").iloc[0]["DENSITY_CHANGE_PCT"]:.1f}%)'
    )

    analysis_time = time.time() - start_time
    print(f'✅ Analysis completed in {analysis_time:.1f} seconds\n')

    # Phase 3: AI Integration (10 minutes)
    print('🤖 PHASE 3: AI-Powered Insights Generation (10 minutes)')
    print('-' * 50)
    start_time = time.time()

    print('• Preparing structured data for AI analysis...')
    print('• Generating planning insights using OpenAI...')
    insights = analyzer.generate_ai_insights()

    print('• AI-Generated Planning Recommendations:')
    for i, insight in enumerate(insights[:3], 1):
        print(f'  {i}. {insight}')

    ai_time = time.time() - start_time
    print(f'✅ AI insights generated in {ai_time:.1f} seconds\n')

    # Phase 4: Visualization (10 minutes)
    print('📈 PHASE 4: Interactive Visualizations (10 minutes)')
    print('-' * 50)
    start_time = time.time()

    print('• Creating choropleth maps showing density changes...')
    analyzer.create_visualizations()
    print('• Generating interactive web map with municipality details...')
    analyzer.create_interactive_map()
    print('• Interactive map available at: interactive_map.html')

    viz_time = time.time() - start_time
    print(f'✅ Visualizations created in {viz_time:.1f} seconds\n')

    # Phase 5: Report Generation (5 minutes)
    print('📄 PHASE 5: Automated Report Generation (5 minutes)')
    print('-' * 50)
    start_time = time.time()

    print('• Compiling analysis results into professional report...')
    analyzer.generate_report()
    print('• Embedding visualizations and AI insights...')
    print('• Report generated: municipal_planning_report.pdf')

    report_time = time.time() - start_time
    print(f'✅ Report generated in {report_time:.1f} seconds\n')

    # Summary
    total_time = setup_time + analysis_time + ai_time + viz_time + report_time
    print('🎯 WORKSHOP SUMMARY')
    print('=' * 60)
    print(f'Total processing time: {total_time:.1f} seconds')
    print(f'Municipalities analyzed: {len(comparison)}')
    print(f'Average growth rate: {stats["mean_change_pct"]:.1f}%')
    print(f'AI recommendations: {len(insights)}')
    print('\n📁 Generated Files:')
    print('  • density_maps.png - Choropleth maps')
    print('  • analysis_summary.png - Statistical summaries')
    print('  • interactive_map.html - Interactive web map')
    print('  • municipal_planning_report.pdf - Complete report')

    print('\n🎉 Workshop demonstration completed successfully!')
    print('Files are ready for audience exploration and municipal planning use.')


if __name__ == '__main__':
    workshop_demo()
