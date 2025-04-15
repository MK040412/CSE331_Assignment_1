import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from matplotlib.font_manager import FontProperties
import matplotlib.ticker as ticker
from matplotlib import rcParams

plt.style.use('seaborn-v0_8-whitegrid')
rcParams['font.family'] = 'serif'
rcParams['font.serif'] = ['Times New Roman']
rcParams['font.size'] = 10
rcParams['axes.labelsize'] = 11
rcParams['axes.titlesize'] = 12
rcParams['xtick.labelsize'] = 9
rcParams['ytick.labelsize'] = 9
rcParams['legend.fontsize'] = 9
rcParams['figure.titlesize'] = 14
rcParams['figure.figsize'] = (8, 6)
rcParams['savefig.dpi'] = 300
rcParams['savefig.bbox'] = 'tight'
rcParams['axes.grid'] = True
rcParams['grid.alpha'] = 0.3
rcParams['axes.axisbelow'] = True
rcParams['axes.linewidth'] = 0.8
rcParams['axes.edgecolor'] = '#333333'


grayscale_colors = ['#000000', '#333333', '#666666', '#999999', '#CCCCCC', '#EEEEEE']
hatches = ['', '/', '\\', 'x', '.', 'o', '*', '+', '-', '|']

def load_data(filename):
    df = pd.read_csv(filename)
    df.columns = ['Algorithm', 'DatasetType', 'DataSize', 'AvgExecutionTime', 
                 'Comparisons', 'Assignments', 'MemoryUsage', 
                 'TheoreticalStability', 'MeasuredStability']
    
    stability_mapping = {
        '안정': 'Stable',
        '불안정': 'Unstable'
    }
    
    if any(df['TheoreticalStability'].isin(stability_mapping.keys())):
        df['TheoreticalStability'] = df['TheoreticalStability'].map(stability_mapping).fillna(df['TheoreticalStability'])
        df['MeasuredStability'] = df['MeasuredStability'].map(stability_mapping).fillna(df['MeasuredStability'])
    
    return df

def group_algorithms(df):
    conventional = ['Bubble Sort', 'Insertion Sort', 'Selection Sort', 
                    'Quick Sort', 'Merge Sort', 'Heap Sort']
    contemporary = ['Library Sort', 'Tim Sort', 'Cocktail Sort', 
                    'Comb Sort', 'Tournament Sort', 'Intro Sort']
    
    df['AlgorithmType'] = df['Algorithm'].apply(
        lambda x: 'Conventional' if x in conventional else 'Contemporary')
    
    return df

def prepare_data(filename):
    df = load_data(filename)
    df = group_algorithms(df)
    return df

def plot_execution_time_by_size(df):
    sizes = sorted(df['DataSize'].unique())
    
    plt.figure(figsize=(10, 8))
    
    for i, size in enumerate(sizes):
        data = df[df['DataSize'] == size]
        
        ax = plt.subplot(len(sizes), 1, i+1)
        
        # 실행 시간 기준으로 정렬
        data = data.sort_values('AvgExecutionTime')
        
        # 알고리즘 유형에 따라 패턴 지정
        colors = ['black'] * len(data)
        hatches_list = []
        for _, row in data.iterrows():
            if row['AlgorithmType'] == 'Conventional':
                hatches_list.append('')
            else:
                hatches_list.append('/')
        
        bars = ax.barh(data['Algorithm'], data['AvgExecutionTime'], color=colors, edgecolor='black')
        
        # 패턴 적용
        for bar, hatch in zip(bars, hatches_list):
            bar.set_hatch(hatch)
        
        # 값 레이블 추가
        for bar in bars:
            width = bar.get_width()
            label_x_pos = width * 1.01
            ax.text(label_x_pos, bar.get_y() + bar.get_height()/2, f'{width:.1f}ms',
                   va='center', fontsize=8)
        
        ax.set_title(f'Data Size: {size:,}', fontsize=10)
        ax.set_xlabel('Average Execution Time (ms)')
        
        # 그리드 설정
        ax.grid(axis='x', linestyle='--', alpha=0.3)
        
        # 알고리즘 유형 표시를 위한 범례
        if i == 0:
            from matplotlib.patches import Patch
            legend_elements = [
                Patch(facecolor='black', label='Conventional'),
                Patch(facecolor='black', hatch='/', label='Contemporary')
            ]
            ax.legend(handles=legend_elements, loc='upper right')
    
    plt.suptitle('Sorting Algorithm Performance: Execution Time by Data Size', 
                fontsize=14, fontweight='bold')
    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.savefig('execution_time_by_size.png')
    plt.close()


def plot_performance_heatmap(df):
    # 가장 큰 데이터 크기만 선택
    max_size = df['DataSize'].max()
    data = df[df['DataSize'] == max_size]
    
    # "Reverse Sorted" 데이터 타입 제외
    data = data[data['DatasetType'] != 'Reverse Sorted']
    
    plt.figure(figsize=(12, 8))
    
    # 피벗 테이블 생성
    pivot = data.pivot(index='Algorithm', columns='DatasetType', values='AvgExecutionTime')
    
    # 알고리즘 유형에 따라 정렬
    conv_algorithms = pivot.index[pivot.index.isin(data[data['AlgorithmType'] == 'Conventional']['Algorithm'])]
    cont_algorithms = pivot.index[pivot.index.isin(data[data['AlgorithmType'] == 'Contemporary']['Algorithm'])]
    
    # 알고리즘 유형별로 정렬된 새 인덱스 생성
    new_index = list(conv_algorithms) + list(cont_algorithms)
    pivot = pivot.reindex(new_index)
    
    # 순수 흑백 히트맵 생성
    ax = sns.heatmap(pivot, annot=True, fmt='.1f', cmap='binary', linewidths=0.5)
    
    # 알고리즘 유형 구분선 추가
    ax.axhline(len(conv_algorithms), color='black', lw=2)
    
    # 컬러바 레이블 추가
    cbar = ax.collections[0].colorbar
    cbar.set_label('Average Execution Time (ms)')
    
    plt.title(f'Algorithm Performance by Dataset Type (n={max_size:,})',
              fontsize=14, fontweight='bold')
    plt.tight_layout()
    plt.savefig('performance_heatmap.png')
    plt.close()

def plot_comparisons_vs_assignments(df):
    # 가장 큰 데이터 크기만 선택
    max_size = df['DataSize'].max()
    data = df[df['DataSize'] == max_size]
    
    # 데이터 타입별로 그룹화
    dataset_types = data['DatasetType'].unique()
    
    # 표 형식으로 시각화
    fig, axes = plt.subplots(len(dataset_types), 1, figsize=(12, 4*len(dataset_types)))
    
    if len(dataset_types) == 1:
        axes = [axes]  # 단일 데이터 타입인 경우 리스트로 변환
    
    for i, dataset_type in enumerate(dataset_types):
        # 현재 데이터 타입에 대한 데이터 필터링
        type_data = data[data['DatasetType'] == dataset_type]
        
        # 알고리즘 유형별로 정렬
        type_data = type_data.sort_values(['AlgorithmType', 'Algorithm'])
        
        # 표 데이터 준비
        table_data = []
        for _, row in type_data.iterrows():
            table_data.append([
                row['Algorithm'], 
                row['AlgorithmType'],
                f"{row['Comparisons']:,.0f}", 
                f"{row['Assignments']:,.0f}"
            ])
        
        # 표 생성
        ax = axes[i]
        ax.axis('tight')
        ax.axis('off')
        table = ax.table(
            cellText=table_data,
            colLabels=['Algorithm', 'Type', 'Comparisons', 'Assignments'],
            loc='center',
            cellLoc='center'
        )
        
        # 표 스타일 조정
        table.auto_set_font_size(False)
        table.set_fontsize(9)
        table.scale(1, 1.5)
        
        # 헤더 행 스타일 조정
        for j, cell in enumerate(table._cells[(0, j)] for j in range(4)):
            cell.set_text_props(weight='bold')
            cell.set_facecolor('#f0f0f0')
        
        # 알고리즘 유형에 따라 행 색상 지정
        for j, row in enumerate(table_data, 1):
            if row[1] == 'Conventional':
                for k in range(4):
                    table._cells[(j, k)].set_facecolor('#ffffff')
            else:
                for k in range(4):
                    table._cells[(j, k)].set_facecolor('#f9f9f9')
        
        # 제목 추가
        ax.set_title(f'Dataset Type: {dataset_type}', pad=20, fontweight='bold')
    
    plt.suptitle(f'Comparisons and Assignments by Algorithm (Data Size: {max_size:,})', 
                 fontsize=16, fontweight='bold', y=0.99)
    plt.tight_layout()
    plt.subplots_adjust(top=0.95)
    plt.savefig('comparisons_vs_assignments_table.png', bbox_inches='tight')
    plt.close()


def plot_memory_usage(df):
    # 가장 큰 데이터 크기만 선택
    max_size = df['DataSize'].max()
    data = df[df['DataSize'] == max_size]
    
    # 메모리 사용량 기준으로 정렬
    data = data.sort_values('MemoryUsage')
    
    plt.figure(figsize=(10, 8))
    
    # 알고리즘 유형에 따라 패턴 지정
    hatches_list = []
    for _, row in data.iterrows():
        if row['AlgorithmType'] == 'Conventional':
            hatches_list.append('')
        else:
            hatches_list.append('/')
    
    # 막대 그래프 생성
    bars = plt.barh(data['Algorithm'], data['MemoryUsage'] / 1024, color='black', edgecolor='black')
    
    # 패턴 적용
    for bar, hatch in zip(bars, hatches_list):
        bar.set_hatch(hatch)
    
    # 값 레이블 추가
    for bar in bars:
        width = bar.get_width()
        label_x_pos = width * 1.01
        plt.text(label_x_pos, bar.get_y() + bar.get_height()/2, f'{width:.1f} KB',
               va='center', fontsize=9)
    
    # 알고리즘 유형 표시를 위한 범례
    from matplotlib.patches import Patch
    legend_elements = [
        Patch(facecolor='black', label='Conventional'),
        Patch(facecolor='black', hatch='/', label='Contemporary')
    ]
    plt.legend(handles=legend_elements, loc='lower right')
    
    plt.title(f'Memory Usage by Algorithm (Data Size: {max_size:,})',
             fontsize=14, fontweight='bold')
    plt.xlabel('Memory Usage (KB)')
    plt.grid(axis='x', linestyle='--', alpha=0.3)
    plt.tight_layout()
    
    plt.savefig('memory_usage.png')
    plt.close()

def plot_stability_comparison(df):
    # 알고리즘별 중복 제거
    stability_data = df.drop_duplicates(subset=['Algorithm'])[
        ['Algorithm', 'AlgorithmType', 'TheoreticalStability', 'MeasuredStability']]
    
    # 안정성을 숫자로 변환 (Stable=1, Unstable=0)
    stability_data['TheoreticalStability'] = stability_data['TheoreticalStability'].apply(
        lambda x: 1 if x == 'Stable' else 0)
    stability_data['MeasuredStability'] = stability_data['MeasuredStability'].apply(
        lambda x: 1 if x == 'Stable' else 0)
    
    # 알고리즘 유형별로 정렬
    stability_data = stability_data.sort_values(['AlgorithmType', 'Algorithm'])
    
    plt.figure(figsize=(12, 6))
    
    x = np.arange(len(stability_data))
    width = 0.35
    
    # 이론적 안정성과 측정된 안정성 막대 그래프 (흑백 스타일)
    plt.bar(x - width/2, stability_data['TheoreticalStability'], width, 
           label='Theoretical Stability', color='black')
    plt.bar(x + width/2, stability_data['MeasuredStability'], width, 
           label='Measured Stability', color='grey', edgecolor='black')
    
    # 불일치 표시
    for i, row in stability_data.iterrows():
        if row['TheoreticalStability'] != row['MeasuredStability']:
            plt.text(x[stability_data.index.get_loc(i)], 0.5, '✗',
                    ha='center', va='center', color='black', fontsize=15, fontweight='bold')
    
    # 알고리즘 유형 구분선 추가
    conv_count = len(stability_data[stability_data['AlgorithmType'] == 'Conventional'])
    plt.axvline(x=conv_count - 0.5, color='black', linestyle='--', alpha=0.5)
    
    # 알고리즘 유형 레이블 추가
    plt.text(conv_count/2 - 0.5, -0.15, 'Conventional', 
             ha='center', fontsize=10, fontweight='bold')
    plt.text(conv_count + (len(stability_data) - conv_count)/2 - 0.5, -0.15, 'Contemporary', 
             ha='center', fontsize=10, fontweight='bold')
    
    plt.xlabel('Algorithm')
    plt.ylabel('Stability')
    plt.title('Theoretical vs Measured Stability by Algorithm',
             fontsize=14, fontweight='bold')
    plt.xticks(x, stability_data['Algorithm'], rotation=45, ha='right')
    plt.yticks([0, 1], ['Unstable', 'Stable'])
    plt.legend(loc='upper right')
    plt.grid(axis='y', linestyle='--', alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('stability_comparison.png')
    plt.close()

def plot_radar_chart(df):
    # 가장 큰 데이터 크기만 선택
    max_size = df['DataSize'].max()
    data = df[df['DataSize'] == max_size]
    
    # 상위 6개 알고리즘만 선택 (가독성을 위해)
    # 각 데이터셋 유형별 평균 실행 시간 계산
    avg_times = data.groupby('Algorithm')['AvgExecutionTime'].mean().sort_values()
    top_algorithms = avg_times.index[:6].tolist()
    
    # 선택된 알고리즘만 필터링
    data = data[data['Algorithm'].isin(top_algorithms)]
    
    # 데이터셋 유형 목록
    dataset_types = data['DatasetType'].unique()
    
    # 레이더 차트 생성
    plt.figure(figsize=(10, 8))
    
    # 각도 계산
    angles = np.linspace(0, 2*np.pi, len(dataset_types), endpoint=False).tolist()
    angles += angles[:1]  # 원형을 닫기 위해 첫 각도 추가
    
    # 축 설정
    ax = plt.subplot(111, polar=True)
    
    # 각 알고리즘에 대한 그래프 그리기
    line_styles = ['-', '--', '-.', ':', '-', '--']
    markers = ['o', 's', '^', 'D', 'v', 'p']
    
    for i, algorithm in enumerate(top_algorithms):
        values = []
        for dataset_type in dataset_types:
            # 이 알고리즘과 데이터셋 유형에 대한 실행 시간 가져오기
            execution_time = data[(data['Algorithm'] == algorithm) & 
                                 (data['DatasetType'] == dataset_type)]['AvgExecutionTime'].values[0]
            values.append(execution_time)
        
        # 원형을 닫기 위해 첫 값 추가
        values += values[:1]
        
        # 그래프 그리기 (흑백 스타일)
        ax.plot(angles, values, linewidth=2, linestyle=line_styles[i], 
               marker=markers[i], color='black', alpha=0.7, label=algorithm)
    
    # 축 레이블 설정
    plt.xticks(angles[:-1], dataset_types, fontsize=9)
    
    # 제목 및 범례
    plt.title(f'Algorithm Performance by Dataset Type (Data Size: {max_size:,})',
             fontsize=14, fontweight='bold')
    plt.legend(loc='upper right', bbox_to_anchor=(0.1, 0.1))
    
    plt.tight_layout()
    plt.savefig('radar_chart.png')
    plt.close()

def plot_efficiency_index(df):

    max_size = df['DataSize'].max()
    data = df[df['DataSize'] == max_size]
    

    data_normalized = data.copy()
    for metric in ['AvgExecutionTime', 'Comparisons', 'Assignments', 'MemoryUsage']:
        max_val = data[metric].max()
        data_normalized[metric] = data[metric] / max_val
    

    data_normalized['EfficiencyIndex'] = (
        data_normalized['AvgExecutionTime'] * 0.4 +
        data_normalized['Comparisons'] * 0.2 +
        data_normalized['Assignments'] * 0.2 +
        data_normalized['MemoryUsage'] * 0.2
    )
    

    data_normalized = data_normalized.sort_values('EfficiencyIndex')
    
    plt.figure(figsize=(10, 8))
    

    hatches_list = []
    for _, row in data_normalized.iterrows():
        if row['AlgorithmType'] == 'Conventional':
            hatches_list.append('')
        else:
            hatches_list.append('/')
    

    bars = plt.barh(data_normalized['Algorithm'], data_normalized['EfficiencyIndex'], 
                   color='black', edgecolor='black')
    

    for bar, hatch in zip(bars, hatches_list):
        bar.set_hatch(hatch)
    

    for bar in bars:
        width = bar.get_width()
        label_x_pos = width * 1.01
        plt.text(label_x_pos, bar.get_y() + bar.get_height()/2, f'{width:.2f}',
               va='center', fontsize=9)
    

    from matplotlib.patches import Patch
    legend_elements = [
        Patch(facecolor='black', label='Conventional'),
        Patch(facecolor='black', hatch='/', label='Contemporary')
    ]
    plt.legend(handles=legend_elements, loc='lower right')
    
    plt.title(f'Algorithm Efficiency Index (Data Size: {max_size:,})\nLower is better',
             fontsize=14, fontweight='bold')
    plt.xlabel('Efficiency Index (normalized)')
    plt.grid(axis='x', linestyle='--', alpha=0.3)
    plt.tight_layout()
    
    plt.savefig('efficiency_index.png')
    plt.close()

def create_complexity_table(df):

    algorithms_data = df.drop_duplicates(subset=['Algorithm', 'AlgorithmType'])[
        ['Algorithm', 'AlgorithmType']]
    

    complexity_info = {
        'Bubble Sort': {'Best': 'O(n)', 'Average': 'O(n²)', 'Worst': 'O(n²)'},
        'Insertion Sort': {'Best': 'O(n)', 'Average': 'O(n²)', 'Worst': 'O(n²)'},
        'Selection Sort': {'Best': 'O(n²)', 'Average': 'O(n²)', 'Worst': 'O(n²)'},
        'Quick Sort': {'Best': 'O(n log n)', 'Average': 'O(n log n)', 'Worst': 'O(n²)'},
        'Merge Sort': {'Best': 'O(n log n)', 'Average': 'O(n log n)', 'Worst': 'O(n log n)'},
        'Heap Sort': {'Best': 'O(n log n)', 'Average': 'O(n log n)', 'Worst': 'O(n log n)'},
        'Library Sort': {'Best': 'O(n)', 'Average': 'O(n log n)', 'Worst': 'O(n²)'},
        'Tim Sort': {'Best': 'O(n)', 'Average': 'O(n log n)', 'Worst': 'O(n log n)'},
        'Cocktail Sort': {'Best': 'O(n)', 'Average': 'O(n²)', 'Worst': 'O(n²)'},
        'Comb Sort': {'Best': 'O(n log n)', 'Average': 'O(n²)', 'Worst': 'O(n²)'},
        'Tournament Sort': {'Best': 'O(n log n)', 'Average': 'O(n log n)', 'Worst': 'O(n log n)'},
        'Intro Sort': {'Best': 'O(n log n)', 'Average': 'O(n log n)', 'Worst': 'O(n log n)'}
    }
    
    algorithms_data = algorithms_data.sort_values(['AlgorithmType', 'Algorithm'])
    

    table_data = []
    for _, row in algorithms_data.iterrows():
        algorithm = row['Algorithm']
        complexity = complexity_info.get(algorithm, {'Best': 'N/A', 'Average': 'N/A', 'Worst': 'N/A'})
        row_data = [algorithm, complexity['Best'], complexity['Average'], complexity['Worst']]
        table_data.append(row_data)
    
    plt.figure(figsize=(10, len(algorithms_data) * 0.5 + 2))
    plt.axis('off')
    
    col_labels = ['Algorithm', 'Best Case', 'Average Case', 'Worst Case']
    

    table = plt.table(
        cellText=table_data,
        colLabels=col_labels,
        loc='center',
        cellLoc='center',
        colWidths=[0.4, 0.2, 0.2, 0.2]
    )
    
    table.auto_set_font_size(False)
    table.set_fontsize(10)
    table.scale(1, 1.5)
    

    for (row, col), cell in table.get_celld().items():
        if row == 0:
            cell.set_text_props(fontproperties=FontProperties(weight='bold'))
            cell.set_facecolor('black')
            cell.set_text_props(color='white')
        else:
            if col == 0:
                cell.set_text_props(fontproperties=FontProperties(weight='bold'))
            if row % 2 == 0:
                cell.set_facecolor('#DDDDDD')
            else:
                cell.set_facecolor('#EEEEEE')
    

    conv_count = len(algorithms_data[algorithms_data['AlgorithmType'] == 'Conventional'])
    table.add_cell(conv_count + 0.5, -0.5, 1, 5, text='', loc='center', 
                  facecolor='black', height=0.05)
    

    plt.text(0.1, 0.95, 'Conventional Algorithms', 
             transform=plt.gcf().transFigure, fontsize=12, fontweight='bold')
    plt.text(0.1, 0.95 - (conv_count + 1) * 0.05, 'Contemporary Algorithms', 
             transform=plt.gcf().transFigure, fontsize=12, fontweight='bold')
    
    plt.title('Time Complexity of Sorting Algorithms',
             fontsize=14, fontweight='bold', pad=20)
    plt.tight_layout()
    
    plt.savefig('complexity_table.png')
    plt.close()

def plot_performance_by_dataset_size(df):

    plt.figure(figsize=(12, 8))
    

    line_styles = {
        'Conventional': {'linestyle': '-', 'marker': 'o', 'color': 'black'},
        'Contemporary': {'linestyle': '--', 'marker': 's', 'color': 'black'}
    }
    

    for algorithm in df['Algorithm'].unique():
        algo_data = df[df['Algorithm'] == algorithm]
        algo_type = algo_data['AlgorithmType'].iloc[0]
        

        plt.plot(algo_data['DataSize'], algo_data['AvgExecutionTime'], 
                **line_styles[algo_type], alpha=0.7, label=algorithm)
    
    plt.xscale('log')
    plt.yscale('log')
    plt.xlabel('Dataset Size (n)')
    plt.ylabel('Average Execution Time (ms)')
    plt.title('Algorithm Performance by Dataset Size',
             fontsize=14, fontweight='bold')
    plt.grid(True, which='both', linestyle='--', alpha=0.3)
    
    plt.legend(loc='upper left', ncol=2, fontsize=8)
    
    plt.tight_layout()
    plt.savefig('performance_by_size.png')
    plt.close()

def generate_all_graphs():
    print("Loading and preparing data...")
    df = prepare_data('sorting_summary_10000.csv')
    
    print("Generating execution time comparison graphs...")
    plot_execution_time_by_size(df)
    
    print("Generating performance heatmaps...")
    plot_performance_heatmap(df)
    
    print("Generating comparisons vs assignments graphs...")
    plot_comparisons_vs_assignments(df)
    
    print("Generating memory usage graphs...")
    plot_memory_usage(df)
    
    print("Generating stability comparison graphs...")
    plot_stability_comparison(df)
    
    print("Generating radar charts...")
    plot_radar_chart(df)
    
    print("Generating efficiency index graphs...")
    plot_efficiency_index(df)
    
    print("Creating complexity tables...")
    create_complexity_table(df)
    
    print("Generating performance by dataset size graphs...")
    plot_performance_by_dataset_size(df)
    
    print("All visualizations have been generated successfully.")

if __name__ == "__main__":
    generate_all_graphs()
