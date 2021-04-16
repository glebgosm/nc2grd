            assert( nc_get_var(ncid, time_id, timesteps) );
            for (int n = 0; n < dim_size[dimt]; n++)
                if (timesteps[n] == user_time) {
                    nTime = n; 
                    break;
                }
            delete[] timesteps; timesteps = NULL;