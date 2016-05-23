import sklearn import cluster

def custom_spectral_cluster(similarity):
	clustering = SpectralClustering(cluster_count, affinity='precomputed', eigen_solver='arpack')
    clustering.fit(similarity)
    clusters = clustering.fit_predict(similarity)
    return clusters