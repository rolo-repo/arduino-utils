#ifndef _ScopedGuard_H_
#define _ScopedGuard_H_

// =========================================================================
class ScopedGuardImplBase 
{
public:
    enum ScopedGuardArmStatus
    {
        sgasARMED = 0,
        sgasDISARMED = 1 
    };

protected:

    ~ScopedGuardImplBase()
    {}

    template <typename G>
    static void doExecute(G &i_g) throw() 
    {
        if (!i_g.m_dismissed)
        {
            i_g.execute();
        }
    }
    
    mutable bool m_dismissed;

public:
    ScopedGuardImplBase(int status = ScopedGuardImplBase::sgasARMED) throw() : m_dismissed(status) 
    {}

    void dismiss() const throw() 
    {
        m_dismissed = true;
    }

    void arm() const throw()
    {
        m_dismissed = false;
    }
};


typedef const ScopedGuardImplBase& ScopedGuard;
// =========================================================================

template <typename F>
class ScopedGuardImpl0 : public ScopedGuardImplBase
{
public:
    static ScopedGuardImpl0<F> makeScopedGuard(F fun, int status = ScopedGuardImplBase::sgasARMED)
    {
        return ScopedGuardImpl0<F>(fun, status);
    }

    ~ScopedGuardImpl0() throw() 
    {
        doExecute(*this);
    }

    void execute() 
    {
        fun_();
    }

protected:
    ScopedGuardImpl0(F fun, int status = ScopedGuardImplBase::sgasARMED) : ScopedGuardImplBase(status), fun_(fun) 
    {}

    F fun_;
};

template <typename F> 
inline ScopedGuardImpl0<F> makeScopedGuard(F fun, int status = ScopedGuardImplBase::sgasARMED)
{
    return ScopedGuardImpl0<F>::makeScopedGuard(fun, status);
}
// =========================================================================


template <typename F, typename P1>
class ScopedGuardImpl1 : public ScopedGuardImplBase
{
public:
    static ScopedGuardImpl1<F, P1> makeScopedGuard(F fun, P1 p1, int status = ScopedGuardImplBase::sgasARMED)
    {
        return ScopedGuardImpl1<F, P1>(fun, p1, status);
    }

    ~ScopedGuardImpl1() throw() 
    {
        doExecute(*this);
    }

    void execute()
    {
        fun_(p1_);
    }

protected:
    ScopedGuardImpl1(F fun, P1 p1, int status = ScopedGuardImplBase::sgasARMED) :
         ScopedGuardImplBase(status), fun_(fun), p1_(p1) 
    {}

    F fun_;
    const P1 p1_;
};

template <typename F, typename P1> 
inline ScopedGuardImpl1<F, P1> makeScopedGuard(F fun, P1 p1, int status = ScopedGuardImplBase::sgasARMED)
{
    return ScopedGuardImpl1<F, P1>::makeScopedGuard(fun, p1, status);
}

// =========================================================================


template <typename F, typename P1, typename P2>
class ScopedGuardImpl2: public ScopedGuardImplBase
{
public:
    static ScopedGuardImpl2<F, P1, P2> makeScopedGuard(F fun, P1 p1, P2 p2, int status = ScopedGuardImplBase::sgasARMED)
    {
        return ScopedGuardImpl2<F, P1, P2>(fun, p1, p2, status);
    }

    ~ScopedGuardImpl2() throw() 
    {
        doExecute(*this);
    }

    void execute()
    {
        fun_(p1_, p2_);
    }

protected:
    ScopedGuardImpl2(F fun, P1 p1, P2 p2, int status = ScopedGuardImplBase::sgasARMED) : ScopedGuardImplBase(status), fun_(fun), p1_(p1), p2_(p2) 
    {}

    F fun_;
    const P1 p1_;
    const P2 p2_;
};

template <typename F, typename P1, typename P2>
inline ScopedGuardImpl2<F, P1, P2> makeScopedGuard(F fun, P1 p1, P2 p2, int status = ScopedGuardImplBase::sgasARMED)
{
    return ScopedGuardImpl2<F, P1, P2>::makeScopedGuard(fun, p1, p2, status);
}

// =========================================================================


template <typename F, typename P1, typename P2, typename P3>
class ScopedGuardImpl3 : public ScopedGuardImplBase
{
public:
    static ScopedGuardImpl3<F, P1, P2, P3> makeScopedGuard(F fun, P1 p1, P2 p2, P3 p3, int status = ScopedGuardImplBase::sgasARMED)
    {
        return ScopedGuardImpl3<F, P1, P2, P3>(fun, p1, p2, p3, status);
    }

    ~ScopedGuardImpl3() throw() 
    {
        doExecute(*this);
    }

    void execute()
    {
        fun_(p1_, p2_, p3_);
    }

protected:
    ScopedGuardImpl3(F fun, P1 p1, P2 p2, P3 p3, int status = ScopedGuardImplBase::sgasARMED) : ScopedGuardImplBase(status), fun_(fun), p1_(p1), p2_(p2), p3_(p3) 
    {}

    F fun_;
    const P1 p1_;
    const P2 p2_;
    const P3 p3_;
};

template <typename F, typename P1, typename P2, typename P3>
inline ScopedGuardImpl3<F, P1, P2, P3> makeScopedGuard(F fun, P1 p1, P2 p2, P3 p3, int status = ScopedGuardImplBase::sgasARMED)
{
    return ScopedGuardImpl3<F, P1, P2, P3>::makeScopedGuard(fun, p1, p2, p3, status);
}

// =========================================================================
// =========================================================================

template <class Obj, typename MemFun>
class ObjScopedGuardImpl0 : public ScopedGuardImplBase
{
public:
    static ObjScopedGuardImpl0<Obj, MemFun> makeObjGuard(Obj& obj, MemFun memFun, int status = ScopedGuardImplBase::sgasARMED)
    {
        return ObjScopedGuardImpl0<Obj, MemFun>(obj, memFun, status);
    }

    ~ObjScopedGuardImpl0() throw() 
    {
        doExecute(*this);
    }

    void execute() 
    {
        (obj_.*memFun_)();
    }

protected:
    ObjScopedGuardImpl0(Obj& obj, MemFun memFun, int status = ScopedGuardImplBase::sgasARMED) : ScopedGuardImplBase(status), obj_(obj), memFun_(memFun) 
    {}

    Obj& obj_;
    MemFun memFun_;
};

template <class Obj, typename MemFun>
inline ObjScopedGuardImpl0<Obj, MemFun> makeObjGuard(Obj& obj, MemFun memFun, int status = ScopedGuardImplBase::sgasARMED)
{
    return ObjScopedGuardImpl0<Obj, MemFun>::makeObjGuard(obj, memFun, status);
}

template <class Obj, typename F>
inline ObjScopedGuardImpl0<Obj, F> makeObjGuard1(F i_func, int status = ScopedGuardImplBase::sgasARMED)
{
    return ObjScopedGuardImpl0<Obj, F>::makeObjGuard(i_func, status);
}

// =========================================================================


template <typename Ret, class Obj1, class Obj2>
inline ObjScopedGuardImpl0<Obj1,Ret(Obj2::*)()> makeScopedGuard(Ret(Obj2::*memFun)(), Obj1 &obj, int status = ScopedGuardImplBase::sgasARMED) 
{
  return ObjScopedGuardImpl0<Obj1,Ret(Obj2::*)()>::makeObjGuard(obj, memFun, status);
}

// =========================================================================

template <typename Ret, class Obj1, class Obj2>
inline ObjScopedGuardImpl0<Obj1,Ret(Obj2::*)()> makeScopedGuard(Ret(Obj2::*memFun)(), Obj1 *obj, int status = ScopedGuardImplBase::sgasARMED) 
{
  return ObjScopedGuardImpl0<Obj1,Ret(Obj2::*)()>::makeObjGuard(*obj, memFun, status);
}

// =========================================================================

template <class Obj, typename MemFun, typename P1>
class ObjScopedGuardImpl1 : public ScopedGuardImplBase
{
public:
    static ObjScopedGuardImpl1<Obj, MemFun, P1> makeObjGuard(Obj& obj, MemFun memFun, P1 p1, int status = ScopedGuardImplBase::sgasARMED)
    {
        return ObjScopedGuardImpl1<Obj, MemFun, P1>(obj, memFun, p1, status);
    }

    ~ObjScopedGuardImpl1() throw() 
    {
        doExecute(*this);
    }

    void execute() 
    {
        (obj_.*memFun_)(p1_);
    }

protected:
    ObjScopedGuardImpl1(Obj& obj, MemFun memFun, P1 p1, int status = ScopedGuardImplBase::sgasARMED) : ScopedGuardImplBase(status), obj_(obj), memFun_(memFun), p1_(p1) 
    {}
    
    Obj& obj_;
    MemFun memFun_;
    const P1 p1_;
};

// =========================================================================


template <class Obj, typename MemFun, typename P1>
inline ObjScopedGuardImpl1<Obj, MemFun, P1> makeObjGuard(Obj& obj, MemFun memFun, P1 p1, int status = ScopedGuardImplBase::sgasARMED)
{
    return ObjScopedGuardImpl1<Obj, MemFun, P1>::makeObjGuard(obj, memFun, p1, status);
}

// =========================================================================

template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b>
inline ObjScopedGuardImpl1<Obj1,Ret(Obj2::*)(P1a),P1b> makeScopedGuard(Ret(Obj2::*memFun)(P1a), Obj1 &obj, P1b p1, int status = ScopedGuardImplBase::sgasARMED) 
{
  return ObjScopedGuardImpl1<Obj1,Ret(Obj2::*)(P1a),P1b>::makeObjGuard(obj,memFun, p1, status);
}

// =========================================================================

template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b>
inline ObjScopedGuardImpl1<Obj1,Ret(Obj2::*)(P1a),P1b> makeScopedGuard(Ret(Obj2::*memFun)(P1a), Obj1 *obj, P1b p1, int status = ScopedGuardImplBase::sgasARMED) 
{
  return ObjScopedGuardImpl1<Obj1,Ret(Obj2::*)(P1a),P1b>::makeObjGuard(*obj,memFun, p1, status);
}

// =========================================================================


template <class Obj, typename MemFun, typename P1, typename P2>
class ObjScopedGuardImpl2 : public ScopedGuardImplBase
{
public:
    static ObjScopedGuardImpl2<Obj, MemFun, P1, P2> makeObjGuard(Obj& obj, MemFun memFun, P1 p1, P2 p2, int status = ScopedGuardImplBase::sgasARMED)
    {
        return ObjScopedGuardImpl2<Obj, MemFun, P1, P2>(obj, memFun, p1, p2, status);
    }

    ~ObjScopedGuardImpl2() throw() 
    {
        doExecute(*this);
    }

    void execute() 
    {
        (obj_.*memFun_)(p1_, p2_);
    }

protected:
    ObjScopedGuardImpl2(Obj& obj, MemFun memFun, P1 p1, P2 p2, int status = ScopedGuardImplBase::sgasARMED) : ScopedGuardImplBase(status), obj_(obj), memFun_(memFun), p1_(p1), p2_(p2) 
    {}

    Obj& obj_;
    MemFun memFun_;
    const P1 p1_;
    const P2 p2_;
};

// =========================================================================

template <class Obj, typename MemFun, typename P1, typename P2>
inline ObjScopedGuardImpl2<Obj, MemFun, P1, P2> makeObjGuard(Obj& obj, MemFun memFun, P1 p1, P2 p2, int status = ScopedGuardImplBase::sgasARMED)
{
    return ObjScopedGuardImpl2<Obj, MemFun, P1, P2>::makeObjGuard(obj, memFun, p1, p2, status);
}

// =========================================================================

template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b, typename P2a, typename P2b>
inline ObjScopedGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a),P1b,P2b> makeScopedGuard(Ret(Obj2::*memFun)(P1a,P2a), Obj1 &obj, P1b p1, P2b p2, int status = ScopedGuardImplBase::sgasARMED) 
{
  return ObjScopedGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a),P1b,P2b>::makeObjGuard(obj,memFun,p1,p2, status);
}

// =========================================================================

template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b, typename P2a, typename P2b>
inline ObjScopedGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a),P1b,P2b> makeScopedGuard(Ret(Obj2::*memFun)(P1a,P2a), Obj1 *obj, P1b p1, P2b p2, int status = ScopedGuardImplBase::sgasARMED) 
{
  return ObjScopedGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a),P1b,P2b>::makeObjGuard(*obj,memFun,p1,p2,status);
}

#endif 
